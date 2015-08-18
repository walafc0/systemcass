/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 sc_event.cc                       |
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


#include <iostream>

#include "sc_event.h"
#include "sc_interface.h"
#include "sc_port_ext.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {
// ----------------------------------------------------------------------------
//  CLASS : sc_event
//
//  Static events.
// ----------------------------------------------------------------------------

ostream & operator << (ostream & o, const sc_event & s) {
    o << get_name (s.get_interface().get_pointer()) << ".";
    switch (s.get_flag ()) {
        case sc_event::VAL:
            return o << "val()";
        case sc_event::NEG:
            return o << "neg()";
        case sc_event::POS:
            return o << "pos()";
        default:
            cerr << "Internal error : unknown event flag.\n";
            exit (13);
    }
}

sc_event::sc_event (const sc_interface & inter_, flag_t flag_) : interface (inter_) {
    flag = flag_;
}

sc_event::~sc_event() {}

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

