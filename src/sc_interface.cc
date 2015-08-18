/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_interface.cc                   |
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
#include <map>
#include <cstdlib>  //exit

#include "sc_interface.h"
#include "sc_event.h"
#include "assert.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

struct interface_infos_t {
    size_t data_size_in_bytes;
    sc_event default_event;
    interface_infos_t() {
        exit(12);
    }
    interface_infos_t (size_t s, sc_event e) : data_size_in_bytes(s), default_event(e) {}
};

typedef std::map<const sc_interface *,interface_infos_t> interface2infos_t;
static interface2infos_t interface2infos;

}

using namespace sc_core;

// ----------------------------------------------------------------------------
//  CLASS : sc_interface
//
//  
// ----------------------------------------------------------------------------

sc_interface::~sc_interface() {}
  
// constructor 
sc_interface::sc_interface() {
  pointer = NULL; // => not assigned
}


void sc_interface::init (size_t s) const {
    interface2infos_t::value_type pair (this, interface_infos_t(s,sc_event (*this, sc_event::VAL)));
    interface2infos.insert(pair);
}

size_t sc_interface::data_size_in_bytes () const {
    interface2infos_t::iterator i = interface2infos.find(this);
#ifdef CONFIG_DEBUG
    if (i == interface2infos.end()) {
        cerr << "Internal error : can't find data size of " << this << "\n";
        exit (90);
    }
#endif
    return i->second.data_size_in_bytes;
}


const sc_event & sc_interface::default_event () const {
    interface2infos_t::iterator i = interface2infos.find(this);
#ifdef CONFIG_DEBUG
    if (i == interface2infos.end ()) {
        cerr << "Internal error : can't find default event of " << this << "\n";
        exit (90);
    }
#endif
    return i->second.default_event;
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

