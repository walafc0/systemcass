/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  sc_clock_ext.h                   |
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

#ifndef __SC_CLOCK_EXT_H__
#define __SC_CLOCK_EXT_H__

#include "sc_signal.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_clock
//
// ----------------------------------------------------------------------------

class sc_clock : public sc_signal<bool> {
    ///////////
    // Internal
    void init ();
    ///////////

    private:
    typedef bool data_type;
    typedef sc_signal<bool> base_type;
    typedef sc_clock this_type;

    public:
    sc_clock();
    ~sc_clock();
    
    // Changes from LRM : 
    // We use "const char *" instead of "sc_module_name" because we don't need
    // to add a module name in the stack when constructing sc_clock !
    
    explicit sc_clock(const char * name_); 
    sc_clock(const char * name_,
             const sc_time & period_,
             double          duty_cycle_ = 0.5,
             const sc_time & start_time_ = SC_ZERO_TIME,
             bool            posedge_first_ = true);

    sc_clock(const char *name_,
             double period_,
             double duty_cycle_ = 0.5,
             double start_time_ = 0.0,
             bool   posedge_first_ = true);

    static const char * const kind_string;

    /*inline*/ this_type & operator = (const data_type & value_);

    bool posedge_first;
};

} // end of sc_core namespace

#endif /* __SC_CLOCK_EXT_H__ */

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

