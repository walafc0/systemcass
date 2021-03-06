/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 module_hierarchy_ext.h            |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   15_12_2005                      |
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

#ifndef __MODULE_HIERARCHY_EXT_H__
#define __MODULE_HIERARCHY_EXT_H__

#include "sc_module_ext.h"
#include "sc_object.h"

#include <vector>

namespace sc_core {

extern const std::vector<sc_object *>& sc_get_top_level_objects();
extern const sc_object * sc_find_object(const char * name);

} // end of namespace sc_core

#endif // __MODULE_HIERARCHY_H__

