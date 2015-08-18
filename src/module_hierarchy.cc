/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 module_hierarchy.cc               |
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

#include <cassert>
#include <map>
#include <cstdlib>
#include <cstring>

#include "sc_module.h"
#include "module_hierarchy.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


using namespace std;

namespace sc_core {

typedef vector<sc_object *> sc_object_list_t;
typedef map<const sc_object *, sc_object_list_t> sc_object2sc_object_list_t;
typedef map<const sc_object*, sc_object *> sc_object2sc_object_t;

sc_object_list_t top_level_objects;
sc_object2sc_object_list_t object2childs;
sc_object2sc_object_t object2parent;


void set_parent (sc_module &mod, sc_module * parent) {
    object2parent[&mod] = parent; 
    sc_object_list_t &obj_list = (parent == NULL) ? top_level_objects : object2childs[parent];
    obj_list.push_back (&mod);
}


void add_child (sc_object &obj) {
    sc_object_list_t * obj_list;
    if (modules_stack.empty ()) {
        obj_list = &top_level_objects;
    }
    else {
        const sc_module * parent = modules_stack.top ();
        if (parent == NULL) {
            return; //obj_list = &top_level_objects;
        }
        else {
            assert(parent != &obj);
            const sc_object * pobj = (const sc_module *) parent;
            obj_list = &(object2childs[pobj]);
        }
    }
    obj_list->push_back (&obj);
}


const std::vector<sc_object *>& sc_get_top_level_objects() {
    return top_level_objects;
}


const sc_object * sc_find_object(const char * name) {
    sc_object2sc_object_list_t::iterator i = object2childs.begin ();
    while (i != object2childs.end()) {
        const sc_object * obj = i->first;
        const char * n = obj->name ();
        if (strcmp (name, n) == 0) {
            return obj;
        }
        ++i;
    }
    return NULL;
}


const std::vector<sc_object *>& get_child_objects(const sc_object &obj) {
    sc_object_list_t &l = object2childs[&obj];
    /* 
     * If the object is not in the objects list,
     * get_child_objects returns an empty list.
     */
    return l;    
}


sc_object * get_parent_object(const sc_object &obj) {
    return object2parent[&obj];
}

} // end of namespace sc_core


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

