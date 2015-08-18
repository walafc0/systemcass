/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_numrep.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   04_05_2005                      |
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

#include <string>

#include "sc_numrep.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace sc_dt {

const std::string numrep_name[] = {
    "SC_DEC",
    "SC_BIN",
    "SC_BIN_US",
    "SC_BIN_SM",
    "SC_OCT",
    "SC_OCT_US",
    "SC_OCT_SM",
    "SC_HEX",
    "SC_HEX_US",
    "SC_HEX_SM",
    "SC_CSD"
};

const std::string to_string (sc_numrep n) {
    return numrep_name[n];
}


} // end of sc_dt namespace

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

