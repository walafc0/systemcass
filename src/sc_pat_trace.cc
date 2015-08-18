/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_pat_trace.cc                 |
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

#include <ctime>

#include "sc_trace.h"
#include "sc_pat_trace.h"
#include "sc_ver.h"
#include "internal.h" // notrace

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef CONFIG_PAT_TRACE_FORMAT



extern "C" {
#include "genpat.h"
}


#include <cstdlib>
#include <cstdio>


using namespace std;

namespace sc_core {


sc_trace_file * sc_create_pat_trace_file(const char * name) {
    if (notrace) {
        return NULL;
    }
    //création d'1 instance de la structure Sc_trace_file:
    sc_trace_file * traceFic = new sc_trace_file();
    traceFic->flag = PAT_FORMAT;	

    trace_file_list.push_back(traceFic);

    DEF_GENPAT((char *) name);

    return traceFic; 
    //pointeur sur le sc_trace_file dontl'un des elts pointe sur le fichier.pat crée 
}


//*************************************************************************

void sc_close_pat_trace_file(sc_trace_file * traceFic ) {	
    if (notrace) {
        return;
    }

    trace(*traceFic, false);

    SAV_GENPAT();

    //libération de l'instance de la structure en mémoire:
    delete traceFic;

    vector<sc_trace_file *>::iterator i;
    for (i = trace_file_list.begin (); i != trace_file_list.end(); ++i) {
        if (*i == traceFic) {
            trace_file_list.erase(i);
            break;
        }
    }
}

} // end of sc_core namespace

#endif

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

