/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 serializations.cc                 |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   10_01_2006                      |
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
#include <map>
#include <fstream>

#include "internal_ext.h" // tab_t
#include "serialization.h"
#include "entity.h"
#include "sc_module.h"
#include "sc_object.h"
#include "hex2string.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;
using namespace sc_core;


namespace sc_core {


static void dec2string(char * buf, const tab_t * val, int bit_number) {
    assert(bit_number <= 64);
    if (bit_number == 1) {
        bool v = *((const bool *) val);
        sprintf(buf, "%d", (v) ? 1 : 0);
    }
    else if (bit_number <= 8) {
        uint8 v = *((const uint8 *) val);
        int v2 = v;
        sprintf(buf, "%d", v2);
    }
    else if (bit_number <= 16) {
        uint16 v = *((const uint16 *) val);
        sprintf(buf, "%u", (uint32) v);
    }
    else if (bit_number <= 32) {
        uint32 v = *((const uint32 *) val);
        sprintf(buf, "%u", v);
    }
    else if (bit_number <= 64) {
        uint64 v = *((const uint64 *) val);
        sprintf(buf, "%llu", v);
    }
}


static void save_signal_table(ostream & o, int hex = true) {
    const equi_list_t &eq_l = get_equi_list();
    equi_list_t::const_iterator it;
    for (it = eq_l.begin(); it != eq_l.end(); ++it) {
        const equi_t & eq = *it;
        equi_t::const_iterator it2 = eq.begin();
        const entity & en = *it2;
        const sc_interface * f = en.interface;
        o << dec << get_name(eq) << endl;
        char buf[128];
        if (hex) {
            buf[0] = '0';
            buf[1] = 'x';
            hex2string(buf + 2, f->get_pointer(), f->data_size_in_bytes() << 3);
        }
        else {
            dec2string(buf, f->get_pointer(), f->data_size_in_bytes() << 3);
        }
        o << buf;
        o << endl;
    }
}


typedef map<const sc_module *, save_fct_t1> sc_module2save_fct_t1;

sc_module2save_fct_t1 save_handler_table;

void set_save_handler(const sc_module & mod, save_fct_t1 fct) {
    save_handler_table[&mod] = fct;
}


static void save_modules(FILE * o) {
    sc_module2save_fct_t1::const_iterator it;
    for (it = save_handler_table.begin(); it != save_handler_table.end(); ++it) {
        const sc_module * mod = it->first;
        save_fct_t1 fct = it->second;
        assert(mod != NULL);
        fprintf(o, "module\n%s\n", mod->name());
        if (fct != NULL) {
            (((sc_module *) mod)->*fct)(o);
        }
    }
}


void sc_save_simulation(const char * filename) {
    update();
    if (dump_stage) {
        cerr << "Saving simulation into \"" << filename << "\"... ";
    }

    ofstream file;
    file.open(filename);
    file << "CABA Save!" << endl;
    file << (sc_time_stamp() / 1000) << endl;
    file << sc_time_stamp().to_string() << endl;
    save_signal_table(file, true);
    file.close();
    FILE * f = fopen(filename, "a+");
    assert(f != NULL);
    save_modules(f);
    fclose(f);

    if (dump_stage) {
        cerr << "done.\n";
    }
}


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

