/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_main.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
/------------------------------------------------------------*/

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

#ifndef __SYSTEMC_H__
#define __SYSTEMC_H__

#include <systemc>


using sc_core::method_process_t;
using sc_core::sc_port_base;
using sc_core::sc_object;
using sc_core::sc_interface;
using sc_core::sc_module;
using sc_core::sc_module_name;
using sc_core::sc_event;
using sc_core::sc_event_finder;
using sc_core::sc_sensitive;
using sc_core::sc_sensitive_pos;
using sc_core::sc_sensitive_neg;

/////////////////////////////////
// Ports & Signals
/////////////////////////////////
using sc_core::sc_time;
using sc_core::sc_simulation_time;
using sc_core::sc_time_stamp;

using sc_core::sc_in;
using sc_core::sc_inout;
using sc_core::sc_out;
using sc_core::sc_signal;
using sc_core::sc_port_b;
using sc_core::sc_signal_base;
using sc_core::sc_clock;

using sc_core::sc_trace_file;

using sc_core::method_process_list_t;

using sc_dt::sc_bit;
using sc_dt::sc_bv;
using sc_dt::sc_logic;
using sc_dt::sc_lv;
using sc_dt::sc_unsigned;
using sc_dt::sc_signed;
//using sc_dt::sc_int_base;
using sc_dt::sc_int;
using sc_dt::sc_uint;
using sc_dt::sc_bigint;
using sc_dt::sc_biguint;
//using sc_dt::sc_uint_base;

using std::ios;
using std::cerr;

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

