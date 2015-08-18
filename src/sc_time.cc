/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_time.cc                        |
|                                                             |
| Author  :                 Buchmann Richard                  |
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
#include <sys/time.h>
#include <string>
#include <sstream>

#include "sc_time.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


namespace sc_core {


static const char *const unit_repr_string[6] = {
	"FS", "PS", "NS", "US", "MS", "SEC"
};

uint64_t nb_cycles = 0;

const sc_time SC_ZERO_TIME(0, SC_NS);
sc_time SC_CURRENT_TIME(0, SC_NS);


sc_time::sc_time (const sc_time & t) {
    *this = t;
}


sc_time::sc_time (double val, sc_time_unit tu) {
    time = (uint64_t) val;
    unit = tu;
}


sc_time & sc_time::operator = (const sc_time & t) {
    time = t.time;
    unit = t.unit;
    return *this;
}


const std::string sc_time::to_string() const {
    std::ostringstream o;
    o << time << ' ' << unit_repr_string[unit];
    return o.str();
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

