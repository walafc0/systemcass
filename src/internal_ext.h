/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   internal_ext.h                  |
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

#ifndef __INTERNAL_EXT_H__
#define __INTERNAL_EXT_H__

#include <list>

#include "sc_fwd.h"
#include "sc_nbdefs.h"

/*
 * not a boolean for a C-ANSI dynamic link (not C++ !)
 * not in casc namespace because dynamic link support C linkage only
 * */
extern "C" char unstable;

// options
//#define KEEP_GENERATED_CODE
//#define NO_STATIC_SCHEDULE
namespace sc_core {

// Method Type
typedef int  (*evaluation_fct_t) ();

extern evaluation_fct_t func_combinationals;
extern evaluation_fct_t func_simulate_1_cycle;

// Sensitivity List
typedef std::list<sc_event> sensitivity_list_t;

// Flags for sc_initialize, sc_start, sc_stop
extern bool have_to_stop;
extern bool already_initialized;

// Granularity of the posted write stack
#ifdef UINT64
    typedef uint64 tab_t;
#else
    typedef uint32 tab_t;
// typedef smallest_uint tab_t; // signals of 64 bits are wrong in trace file
#endif
typedef tab_t * equi_table_t;
extern equi_table_t equi_table;

// Flags
extern bool dump_stage;
extern bool print_schedule;
extern bool edit_schedule;

}

#endif

