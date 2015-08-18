/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 module_hierarchy.h                |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   15_12_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __MODULE_HIERARCHY_H__
#define __MODULE_HIERARCHY_H__

#include "module_hierarchy_ext.h"

namespace sc_core {

void add_child                      (sc_object &);
void set_parent                     (sc_module &child, sc_module *parent);
const std::vector<sc_object*>& get_child_objects (const sc_object &);
sc_object*                     get_parent_object (const sc_object &);

} // end of namespace sc_core

#endif // __MODULE_HIERARCHY_H__

