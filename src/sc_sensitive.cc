/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_sensitive.cc                   |
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


#include "sc_sensitive.h"
#include "sc_port.h"
#include "sc_event.h"
#include "sc_event_finder.h"
#include "sc_module.h"
#include "internal.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {


void assert_before_init() {
    if (sc_core::already_initialized) {
        exit(14);
    }
}


static void add_sensitivity(const sc_event & s) {
    if (method == NULL) {
        cerr << "declare a process before declaring " << s << " event\n";
        exit(1);
    }
    method->sensitivity_list.push_back(s);
}

}


using namespace sc_core;

// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive
//
//  Static sensitivity class for events.
// ----------------------------------------------------------------------------

sc_sensitive & sc_sensitive::operator << (const sc_port_base & port) {
    sc_event s(port, sc_event::VAL);
    sc_core::add_sensitivity(s);
    return *this;
}


sc_sensitive & sc_sensitive::operator () (const sc_port_base & port) {
    return *this << port;
}


sc_sensitive & sc_sensitive::operator << (const sc_event & e) {
    sc_core::add_sensitivity(e);
    return *this;
}


sc_sensitive & sc_sensitive::operator () (const sc_event & e) {
    return *this << e;
}


sc_sensitive & sc_sensitive::operator << (sc_event_finder & e) {
    return *this << e.port();
}


sc_sensitive & sc_sensitive::operator () (sc_event_finder & e) {
    return *this << e;
}


sc_sensitive & sc_sensitive::operator << (const sc_interface & e) {
    return *this << e.default_event();
}


sc_sensitive & sc_sensitive::operator () (const sc_interface & e) {
    return *this << e;
}




// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_pos
//
//  Static sensitivity class for positive edge events.
// ----------------------------------------------------------------------------


sc_sensitive_pos & sc_sensitive_pos::operator << (const sc_port_base & port) {
    sc_event s(port, sc_event::POS);
    sc_core::add_sensitivity(s);
    return *this;
}


sc_sensitive_pos& sc_sensitive_pos::operator () (const sc_port_base & port) {
    return *this << port;
}



// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_neg
//
//  Static sensitivity class for negative edge events.
// ----------------------------------------------------------------------------

sc_sensitive_neg & sc_sensitive_neg::operator << (const sc_port_base & port) {
    sc_event s(port, sc_event::NEG);
    sc_core::add_sensitivity(s);
    return *this;
}


sc_sensitive_neg & sc_sensitive_neg::operator () (const sc_port_base & port) {
    return *this << port;
}



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

