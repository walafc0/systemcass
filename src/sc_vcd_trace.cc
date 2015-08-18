/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_vcd_trace.cc                   |
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

#include <cstdio>
#include <cassert>
#include <ctime>
#include <string>

#include "sc_trace.h"
#include "sc_vcd_trace.h"
#include "sc_ver.h"
#include "internal.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


using namespace std;

namespace sc_core {

//-----------------------------------------*/

sc_trace_file * sc_create_vcd_trace_file(const char * name) {
    if (notrace) {
        return NULL;
    }
    assert(name != NULL);
    string filename;
    filename = name;
    filename += ".vcd";

    // Création d'une instance de la structure Sc_trace_file:
    sc_trace_file * traceFic = new sc_trace_file();
    traceFic->flag = VCD_FORMAT;

    trace_file_list.push_back(traceFic);

    //en-tête du fichier VCD:
    char entete[] = "$date\n\t%s\n$end\n\n$version\n\t%s\n$end\n\n$timescale\n\t1 ps\n$end\n\n$scope module SystemC $end\n";

    //ouverture du fichier nommé "*name":
    traceFic->pfic = fopen(filename.c_str(), "w+");//on a un pointeur sur le fichier
    if ((traceFic->pfic) == NULL) {
        /* fopen renvoie NULL si erreur */
        fprintf(stderr, "\n\terreur ouverture outVcd\n");
        exit(15);
    }

    char date[128];
    time_t timep = time(NULL);
    const struct tm * tm = localtime(&timep);
    strftime(date, 128, "%A %d %B %y %Z - %R:%S -", tm); 

    //écriture de l'en-tête du format VCD:
    if ((fprintf(traceFic->pfic, entete, date, sc_version())) == 0) {
        /* fprintf renvoie 0 si erreur */
        cerr << "\n\terreur ecriture de l'entete \n";
        exit(2);
    }

    //on retourne un pointeur sur la structure traceFile
    //dont un des éléments pointe sur notre fichier VCD:
    return traceFic;
}


//*************************************************************************

void sc_close_vcd_trace_file(sc_trace_file * traceFic) {
    if (notrace) {
        return;
    }
    if (!traceFic) {
        cerr << "Warning : Unable to close vcd trace file.\n";
        return;
    }

    if (cpt >= trace_start) {
        trace (*traceFic, false);
        cpt++;
        trace (*traceFic, true);
    }

    //fermeture fichier VCD
    if (fclose(traceFic->pfic)) {
        /* fclose renvoie 0 si OK */
        perror("\n\tclosing VCD file.");
        exit(4);
    }

    //libération de l'instance de la structure en mémoire:
    delete traceFic;

    vector<sc_trace_file *>::iterator i;
    for (i = trace_file_list.begin(); i != trace_file_list.end(); ++i) {
        if (*i == traceFic) {
            trace_file_list.erase(i);
            break;
        }
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

