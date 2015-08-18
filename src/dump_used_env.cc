/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                  dump_used_env.cc                 |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   10_MAY_2005                     |
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

#include <cstdlib>

#include "dump_used_env.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

std::string sc_core::get_used_env() {
    const char * compiler = getenv("GCC");
    const char * systemc_dir = getenv("SYSTEMCASS");
    const char * target_arch = getenv("TARGET_ARCH");

    if (compiler == NULL) {
        compiler = "undefined";
    }

    if (systemc_dir == NULL) {
        systemc_dir = "undefined";
    }

    if (target_arch == NULL) {
        target_arch = "undefined";
    }

    std::string used_env;
    used_env += "GCC         = ";
    used_env += compiler;
    used_env += "\nSYSTEMCASS  = ";
    used_env += systemc_dir;
    used_env += "\nTARGET_ARCH = ";
    used_env += target_arch;

    return used_env;
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

