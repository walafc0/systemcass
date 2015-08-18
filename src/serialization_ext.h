/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 serialization_ext.h               |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   10_01_2006                      |
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

#ifndef __SERIALIZATION_EXT_H__
#define __SERIALIZATION_EXT_H__

#include <iostream>

#include "sc_fwd.h"

namespace sc_core {

/////////////////////////
// SYSTEMCASS SPECIFIC //

// Types
typedef void(sc_core::sc_module::*save_fct_t1) (FILE *) const;

// Functions
extern void sc_save_simulation(const char * filename = "savedefault");

} // end of sc_core namespace

/////////////////////////
// SYSTEMCASS SPECIFIC //
using sc_core::sc_save_simulation;

#endif // __SERIALIZATION_EXT_H__

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

