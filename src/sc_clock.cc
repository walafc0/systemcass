/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                  sc_clock.cc                      |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                           Taktak Sami                       |
  |                                                             |
  | Date    :                   09_09_2005                      |
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

#include "sc_clock.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

const char *const sc_clock::kind_string = "sc_clock";

// ----------------------------------------------------------------------------
//  CLASS : sc_clock
//
// ----------------------------------------------------------------------------


sc_clock::sc_clock(): base_type() {
    init ();
    posedge_first = true;
}


sc_clock::sc_clock(const char * name_): base_type(name_) {
    init ();
    posedge_first = true;
}


sc_clock::sc_clock(const char * name_,
        const sc_time & period_,
        double          duty_cycle_,
        const sc_time & start_time_,
        bool            posedge_first_) : base_type(name_) {
    init ();
    assert(period_ == 1);
    assert(duty_cycle_ == 0.5);
    assert(start_time_ == SC_ZERO_TIME);
    posedge_first = posedge_first_;
}


sc_clock::sc_clock(const char * name_,
        double period_,
        double duty_cycle_,
        double start_time_,
        bool   posedge_first_) : base_type(name_) {
    init ();
    assert(period_ == 1);
    assert(duty_cycle_ == 0.5);
    assert(start_time_ == SC_ZERO_TIME);
    posedge_first = posedge_first_;
}


sc_clock::~sc_clock() {}


void sc_clock::init () {
    set_kind(kind_string);
}


sc_clock::this_type & sc_clock::operator = (const data_type & value_) {
    write(value_);
    return *this;
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

