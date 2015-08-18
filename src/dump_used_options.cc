/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  dump_used_options.cc             |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   04_JAN_2005                     |
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

#include "dump_used_options.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace sc_core {

static const char used_options[] =
  "Used Options :\n"
#ifdef USE_PORT_DEPENDENCY
  "USE_PORT_DEPENDENCY, "
#endif
#ifdef DUMP_PORT_DEPENDENCY
  "DUMP_PORT_DEPENDENCY, "
#endif
#ifdef DUMP_SCHEDULE_STATS
  "DUMP_SCHEDULE_STATS, "
#endif
#ifdef NO_STATIC_SCHEDULE
  "NO_STATIC_SCHEDULE, "
#endif
#ifdef KEEP_GENERATED_CODE
  "KEEP_GENERATED_CODE, "
#endif
#ifdef DUMP_COMBINATIONAL_LIST2DOT
  "DUMP_COMBINATIONAL_LIST2DOT, "
#endif
#ifdef PRINT_SCHEDULE
  "PRINT_SCHEDULE, "
#endif
#ifdef DUMP_STAGE
  "DUMP_STAGE, "
#endif
#ifdef CONFIG_DEFAULT_RUNTIME_COMPILATION     
  "CONFIG_DEFAULT_RUNTIME_COMPILATION, "
#endif
#ifdef CONFIG_OS_DARWIN     
  "CONFIG_OS_DARWIN, "
#endif
#ifdef CONFIG_OS_LINUX     
  "CONFIG_OS_LINUX, "
#endif
#ifdef CONFIG_PAT_TRACE_FORMAT     
  "CONFIG_PAT_TRACE_FORMAT, "
#endif
#ifdef CONFIG_DEBUG
  "CONFIG_DEBUG, "          
#endif
#ifdef UINT64              
  "UINT64, "
#endif
#ifdef INIT_SIGNALS_TO_ZERO
  "INIT_SIGNALS_TO_ZERO, "
#endif
#ifdef _OPENMP
  "_OPENMP, "
#endif
  "...";

const char * get_used_options() {
    return used_options;
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


