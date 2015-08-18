/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_trace.cc                     |
|                                                             |
| Author  :                 Kingbo Paul-Jerome                |
|                           Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

/* 
 * This file is part of the Disydent Project
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/disydent
 * E-mail             : mailto:richard.buchmann@lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Disydent is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <cassert>
#include <list>
#include <cstdio>
#include <iostream>
#include <cstring> //strlen

#include "sc_port.h"
#include "sc_clock.h"
#include "sc_trace.h"
#include "sc_interface.h"
#include "bit2string.h"
#include "hex2string.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


//-----------------------------------------

using namespace std;

//-----------------------------------------
#ifdef CONFIG_PAT_TRACE_FORMAT

extern "C" {
#include "genpat.h"
}

#include <cstdlib>
#include <cstdio>

#endif // CONFIG_PAT_TRACE_FORMAT
//-----------------------------------------


namespace sc_core {

vector<sc_trace_file *>  trace_file_list;
uint64 cpt = 0;
uint64 trace_start = 0;


static void init(sc_trace_file & tf) {
    if (dump_stage) {
        cerr << "Initializing trace functions : Looking for clocks...";
    }

    vector<signal2trace>::iterator i;
    i = (tf.sig_list).begin();
    while (i != (tf.sig_list).end()) {
        if (is_clock(*(i->inter))) {
            tf.clk_list.push_back(*i);
            i = tf.sig_list.erase(i);
        } 
        else {
            ++i;
        }
    }
    if (dump_stage) {
        cerr << "Done\n";
    }
}


static void vcd_print_cycle_number(FILE * f,long long unsigned int num) {
    string cycle;

    // affichage des cycles (#500 ou #2500 etc...)
    fprintf(f,"\n#%llu00\n", num * 5);//cycle de simulation
}



//--------------------------------------------------------------------------------------
// trace function
// Called each end of cycle to dump in trace files
void trace_all(bool part) {
    if (trace_file_list.empty()) {
        return;
    }

    if (cpt >= trace_start) {
        vector<sc_trace_file *>::const_iterator ptf;
        for (ptf = trace_file_list.begin(); ptf != trace_file_list.end(); ++ptf) {
            sc_trace_file * tf = *ptf;
            assert(tf != NULL);
            trace(*tf, part);
        }
    }

    cpt++;
}


#ifdef CONFIG_PAT_TRACE_FORMAT
static void pat_set_value(char * buf, const signal2trace & s) {
    if ((s.bit_size % 4) == 0) {
        buf[1] = 'x';
        hex2string(buf + 2, s.inter->get_pointer(), s.bit_size);
    }
    else {
        buf[1] = 'b';
        bit2string(buf + 2, s.inter->get_pointer(), s.bit_size);
    }
}


inline static void affect(const char * cur, const char * nomSig, const char * buf) {
    AFFECT ((char*)cur, (char*)nomSig, (char*)buf );
}



inline static void pat_trace(
        sc_trace_file       & tf, 
        const signal2trace  & s2t, 
        const ostringstream & cur1,
        const ostringstream & cur2) {
    static char buf[50] = "0\0";
    //cout << "Nom du signal: " << s->nomSig;
    pat_set_value(buf, s2t);
    affect(cur1.str().c_str(), s2t.nomSig.c_str(), buf);
    affect(cur2.str().c_str(), s2t.nomSig.c_str(), buf);
}


inline static void pat_trace_clocks(
        sc_trace_file       & tf,
        const ostringstream & cur1,
        const ostringstream & cur2) {
    vector<signal2trace>::const_iterator s;
    for (s = (tf.clk_list).begin(); s != (tf.clk_list).end(); s++) {
        const signal2trace & s2t = *s;
        char * name = (char *) ((s2t.nomSig).c_str());
        const sc_interface * inter = s2t.inter;
        const sc_clock * clk = (const sc_clock *) inter;
        bool  posedge_first = clk->posedge_first;
        char * a, * b;
        if (posedge_first) {
            a = "0B1";
            b = "0B0";
        }
        else {
            a = "0B0";
            b = "0B1"; 
        }
        affect(cur1.str().c_str(), name, a);
        affect(cur2.str().c_str(), name, b);
    }
}


static void pat_trace_init(sc_trace_file & tf) {
    init(tf);
}


static void pat_trace(sc_trace_file & tf, bool part) { 
    if (part) {
        return;
    }
    
    if (cpt == trace_start) {
        pat_trace_init(tf);
    }

    // time counters
    ostringstream cur1, cur2;
    long long unsigned int buf = cpt * 5;
    cur1 << buf << "00";
    cur2 << buf + 5 << "00";

    // affect each signal
    vector<signal2trace>::iterator s;
    for (s = (tf.sig_list).begin(); s != (tf.sig_list).end(); s++) {
        pat_trace(tf, *s, cur1, cur2);
    }
    pat_trace_clocks(tf, cur1, cur2);
}

#else
void pat_trace (sc_trace_file & tf, bool part) {}
#endif // CONFIG_PAT_TRACE_FORMAT


static bool is_modified(const signal2trace & s2t) {
    unsigned int bit_size = s2t.bit_size;
    if (bit_size > 32) {
        const uint64 * const pointer_saved = (const uint64 *) s2t.pointer;
        const uint64 * const pointer_current = (const uint64 *) s2t.inter->get_pointer();
        return (*pointer_saved != *pointer_current);
    }
    else if (bit_size > 16) {
        const uint32 * const pointer_saved = (const uint32 *) s2t.pointer;
        const uint32 * const pointer_current = (const uint32 *) s2t.inter->get_pointer();
        return (*pointer_saved != *pointer_current);
    }
    else {
        const uint16 * const pointer_saved = (const uint16 *) s2t.pointer;
        const uint16 * const pointer_current = (const uint16 *) s2t.inter->get_pointer();
        return (*pointer_saved != *pointer_current);
    }
}


static bool save_modification(signal2trace & s2t) {
    *(s2t.pointer) = *(s2t.inter->get_pointer());
}


static inline void print(sc_trace_file & tf, signal2trace & s2t) {
    char buf[100];
    bit2string(buf + 1, s2t.inter->get_pointer() , s2t.bit_size);

    char * buf2 = strip(buf + 1);
    int len = strlen(buf2);
    if (s2t.bit_size != 1) {
        --buf2;
        buf2[0] = 'b';
        sprintf(buf2 + len + 1, " %s\n", s2t.alias);
    }
    else {
        sprintf(buf2 + len, "%s\n", s2t.alias);
    }
    
    // ecriture liste[i] dans le fichier VCD:
    if ((fprintf(tf.pfic,buf2)) == 0) {
        cerr << "erreur ecriture du couple Valeur/Nom du signal dans le VCD\n";
        exit(16);
    }
}


static inline void vcd_trace(sc_trace_file & tf, signal2trace  & s2t, bool skip = true) {
    if ((skip) && (!is_modified(s2t))) {
        return;
    }
    save_modification(s2t);
    print(tf, s2t);
}


static void vcd_trace_clocks(const sc_trace_file & tf, bool v) {
    vector<signal2trace>::const_iterator i;
    for (i = (tf.clk_list).begin(); i != (tf.clk_list).end(); i++) {
        const signal2trace & s2t   = *i;
        const sc_interface * inter = s2t.inter;
        const sc_clock * clk = (const sc_clock *) inter;
        bool posedge_first = clk->posedge_first;
        fprintf(tf.pfic, "%c%s\n",(v^posedge_first)?'0':'1', i->alias);
    }
}


static tab_t * vcd_signal_table = NULL;


static int vcd_get_size(const signal2trace & s2t) {
#if 0
    cerr << "alias     : " << s2t.alias    << "\n";
    cerr << "bit_size  : " << s2t.bit_size << "\n";
    cerr << "=> #tab_t : " << ((s2t.bit_size - 1) / (sizeof (tab_t)*8)) + 1 << "\n";
#endif
    return ((s2t.bit_size - 1) / (sizeof(tab_t) * 8)) + 1;
}


static int vcd_get_signal_table_size(const sc_trace_file & tf) {
    int total_size = 0;
    vector<signal2trace>::const_iterator i;
    for (i = (tf.sig_list).begin(); i != (tf.sig_list).end(); i++) {
        const signal2trace & s2t = *i;
        total_size += vcd_get_size(s2t);
    }
    return total_size;
}


static void vcd_alloc_signal_table(int size) {
    if (size == 0) {
        vcd_signal_table = NULL;
    }
    else {
        vcd_signal_table = (tab_t *) malloc(sizeof(tab_t) * size);
    }
#ifdef CONFIG_DEBUG
    if (vcd_signal_table == NULL) {
        cerr << "Internal error : Unable to allocate memory for signal table to trace.\n";
        exit(24032005);
    }
#endif
}


static void vcd_bind_to_signal_table(sc_trace_file & tf) {
    tab_t * cur = vcd_signal_table;
    vector<signal2trace>::iterator i;
    for (i = (tf.sig_list).begin(); i != (tf.sig_list).end(); i++) {
        signal2trace & s2t = *i;
        s2t.pointer = cur;
        cur += vcd_get_size(s2t);
#if 0
        std::cout << s2t 
            << "\nget_pointer () => " << hex << s2t.inter->get_pointer () 
            << "\nvcd pointer () => " << hex << s2t.pointer 
            << "\nvcd_get_size () => " << vcd_get_size (s2t)
            << "\n";
#endif
    }
}


static void vcd_build_signal_table(sc_trace_file & tf) {
    int s = vcd_get_signal_table_size(tf);
    vcd_alloc_signal_table(s);
    vcd_bind_to_signal_table(tf);
}


static void vcd_trace_init(sc_trace_file & tf) {
    init(tf);
    fprintf(tf.pfic,"$upscope $end\n$enddefinitions  $end\n\n$comment\nAll initial values are dumped below at time 0 sec = 0 timescale units.\n$end\n\n$dumpvars\n");
    vcd_build_signal_table(tf);

    // dump all var
    vector<signal2trace>::iterator i;
    for(i = (tf.sig_list).begin() ; i != (tf.sig_list).end(); i++) {
        vcd_trace(tf, *i, false);
    }//fin de la liste des signal2trace

    // clocks to trace
    vcd_trace_clocks(tf, true);

    if (cpt == trace_start) {
        fprintf(tf.pfic, "$end\n"); //fin du $dumpvars
    }
}


static void vcd_trace(sc_trace_file & tf, bool part) {
    if (sc_core::cpt == sc_core::trace_start) {
        if (part == false) {
            vcd_trace_init (tf);
        }
    }
    else {
#if defined(CONFIG_DEBUG)
        if (vcd_signal_table == NULL) {
            cerr << "Internal Error : VCD signal table is not yet allocated.\n";
            exit(1042005);
        }
#endif
        vcd_print_cycle_number(tf.pfic,sc_core::cpt);
        // signals to trace 
        if (part == false) {
            vector<signal2trace>::iterator i;
            for (i = tf.sig_list.begin(); i != tf.sig_list.end(); i++) {
                vcd_trace(tf, *i);
            }//fin de la liste des signal2trace
        }
        // clocks to trace
        vcd_trace_clocks(tf, !part);
    }
}


void trace(sc_trace_file & tf, bool part) {
    switch (tf.flag) {
        // fonction trace() pour VCD:
        case VCD_FORMAT:
            vcd_trace(tf, part);
            break;
            //fin de la fonction trace() pour VCD

            /*------------------------------------------------------------------------------------*/

            // fonction trace() pour PAT:
        case PAT_FORMAT:
            pat_trace(tf, part);
            break;
            //fin de la fonction trace() pour PAT
    }
    // fin du switch format pour trace()

}


/*------------------------------------------------------------------------------------*/


static void vcd_sc_trace(sc_trace_file * tf, const signal2trace & t, const std::string & name) {
    //déclaration du signal dans l'en-tête du fichier VCD:
    // exemple de déclarations :
    // $var wire    1  aaa  clk       $end
    // $var wire    1  aab  resetn       $end
    // $var wire    1  aac  _mips0_IT_5       $end

    std::string declaration;
    std::ostringstream buf;
    // begin
    declaration = "$var wire ";
    buf.width(4);
    buf << t.bit_size;
    declaration += buf.str();
    declaration += "  ";
    declaration += t.alias;
    declaration += "  ";
    declaration += name;
    // bit range
    if (t.bit_size != 1) {
        declaration += " [";
        std::ostringstream bit_size;
        bit_size << t.bit_size - 1;
        declaration += bit_size.str();
        declaration += ":0]";
    }
    // end
    declaration += "       $end\n";

    if ((fprintf(tf->pfic,declaration.c_str())) == 0) {
        cerr << "erreur ecriture de declaration du signal sc_signal\n";
        exit(3);
    }
}


static void pat_sc_trace(sc_trace_file * tf, const signal2trace & t, const std::string & name) {
#ifdef CONFIG_PAT_TRACE_FORMAT
    //exemple:
    //DECLAR ("a", ":2", "X", IN, "3  downto 0", "" );
    //DECLAR ("b", ":2", "X", IN, "3  downto 0", "" );
    //DECLAR ("vdd", ":2", "B", IN, "", "" );

    std::string downto;
    const sc_object * obj = (const sc_object *) t.inter;
    const char * obj_kind = obj->kind();
    char * format;
    char * dir;

    if (strstr(obj_kind, "inout"))
        dir = OUT; // Direction is OUT instead of INOUT
    // because port behavior is only seen as an output port.
    else if (strstr (obj_kind, "in")) {
        dir = IN;
    }
    else if (strstr (obj_kind, "out")) {
        dir = OUT;
    }
    else if (obj_kind == sc_clock::kind_string) {
        dir = SIGNAL;
    }
    else if (obj_kind == sc_signal_base::kind_string) {
        dir = REGISTER;
    }
    // to do : dir = REGISTER;
    //         dir = SIGNAL;

    if (t.bit_size % 4 == 0) {
        format = "x";
    }
    else {
        format = "b";
    }

    if (t.bit_size == 1) {
        downto = "";
    }
    else {  
        std::ostringstream nbits;
        nbits << t.bit_size - 1;
        downto = nbits.str();
        downto += " downto 0";
    }

#if 0
    printf ("DECLAR (\"%s\", \":1\", \"%s\", %s, \"%s\", \"\" );\n", 
            name.c_str (),
            format,
            dir,
            downto.c_str());
#endif
    DECLAR ((char *) (name.c_str()), ":1", format, dir, (char *) downto.c_str(), "");
#endif // CONFIG_PAT_TRACE_FORMAT
}


void sc_trace(sc_trace_file * tf, const signal2trace & t, const std::string & name) {
    if (tf == NULL) {
        return;
    }
    if (t.bit_size > 64) {
        cerr << "Warning : tracing functions do not support data types larger than 64 bits.\n";
    }
    if (already_initialized) {
        cerr << "Warning : please call tracing functions BEFORE sc_initialize.\n";
    }
    //ajout du signal dans la liste des signaux à tracer:
    (tf->sig_list).push_back(t);

    switch (tf->flag) {
        case VCD_FORMAT : 
            vcd_sc_trace(tf, t, name);
            break;

        case PAT_FORMAT :
            pat_sc_trace(tf, t, name);
            break;
        default :
            {
                cerr << "Unknown trace format.\n";
                exit(1);
            }
    }// fin switch format pour sc_trace

}

//----------------------------------------------------------------------------

#define DEF_SC_TRACE(T) /*inline \*/                                        \
    void                                                                    \
    sc_trace (sc_trace_file* tf, const T& object, const std::string & name) \
    {                                                                       \
        signal2trace t;                                                     \
        sc_interface * inter = new sc_localvar< T > (object);               \
        t.inter = inter;                                                    \
        t.alias = alias();                                                  \
        t.bit_size = get_bits_number(object);                               \
        t.nomSig = name;                                                    \
        sc_trace (tf, t, name);                                             \
    }


    DEF_SC_TRACE(bool)
    DEF_SC_TRACE(float)
    DEF_SC_TRACE(double)
    DEF_SC_TRACE(unsigned char)
    DEF_SC_TRACE(unsigned short)
    DEF_SC_TRACE(unsigned int)
    DEF_SC_TRACE(unsigned long)
    DEF_SC_TRACE(char)
    DEF_SC_TRACE(short)
    DEF_SC_TRACE(int)
    DEF_SC_TRACE(long)
    DEF_SC_TRACE(uint64)
    DEF_SC_TRACE(int64)

#undef DEF_SC_TRACE

//---------------------------------------------------------------------------

} // end of sc_core namespace


/*
# Local Variables:
# tab-width: 4;
# c-basic-offset: 4;
# c-file-offsets:((innamespace . 0)(inline-open . 0));
# indent-tabs-mode: nil;
# End:
#
# vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
*/

