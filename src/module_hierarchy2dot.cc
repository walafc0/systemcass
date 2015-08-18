/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  module_hierarchy2dot.cc          |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   26_01_2006                      |
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

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include "module_hierarchy.h"
#include "module_hierarchy2dot.h"
#include "sc_fwd.h"
#include "sc_signal.h"
#include "entity.h"
#include <cassert>
#include "internal.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

/*
static
const char*
get_parent_name (const sc_object &obj)
{
  const sc_object *parent = NULL;//get_parent_object (obj);
  if (parent == NULL)
    return "";
  return parent->basename ();
}
*/

typedef set<const char*> node_set_t;
node_set_t node_set;

static
void
print_edge (ostream &o)
{
  typedef map<const char*, const char*> edge_t;
  edge_t edges;
  const equi_list_t &eq_list = get_equi_list ();
  equi_list_t::const_iterator jt;
  for (jt = eq_list.begin (); jt != eq_list.end (); ++jt)
  {
    const equi_t &eq = (*jt);
    sc_port_base *out   = get_out_port (eq);
    if (out == NULL)
      continue;
    if (is_clock (*out))
      continue;
    const sc_module &out_mod = out->get_module ();
    const char *out_mod_name = out_mod.basename ();
    equi_t::const_iterator it;
    for (it = eq.begin (); it != eq.end (); ++it)
    {
      const entity &in_entity  = *it;
      sc_object    *in_obj     = in_entity.object;
      assert(in_obj != NULL);
      const sc_module *in_parent = NULL;
      switch (in_entity.type) {
      case entity::PORT :
        in_parent = &(in_entity.port->get_module ());
        if (&out_mod == in_parent)
          continue;
        break;
      case entity::SIGNAL :
      default :
        continue;
      }
      edges[out_mod_name] = in_parent->basename ();
      node_set.insert(out_mod_name);
      node_set.insert(in_parent->basename ());
    }
  }
#if 0
  const equi_t &signal_eq = *(eq_list.begin ());
  const char *signal_name = get_name (signal_eq);
  o << "edge [label=\"" << signal_name << "\"]\n";
#endif
  edge_t::const_iterator i;
  for (i = edges.begin (); i != edges.end (); ++i)
  {
    o << i->first
      << " -> " 
      << i->second 
      << ";\n";
  }
}

static void print_node (ostream&, const vector<sc_object*>&);

static
void
print_node (ostream         &o,
            const sc_object &obj)
{
  const vector<sc_object*> &obj_list = get_child_objects (obj);
  bool subgraph = (obj_list.empty() == false);
//                  && (get_parent_object (obj) != NULL);
  if (subgraph) {
    const char *name = obj.basename ();
    o << "subgraph \"cluster" << name << "\" {\n"
      << "label=\"" << name << "\";\n";
    print_node (o, obj_list);
    o << "}\n";
  } /*else*/ {
//    if (obj.kind () == sc_module::kind_string)
    if (node_set.find (obj.basename ()) != node_set.end())
      o << obj.basename () << endl;
  }
}

static
void
print_node (ostream                  &o,
            const vector<sc_object*> &obj_list)
{
  vector<sc_object*>::const_iterator it;
  for (it = obj_list.begin(); it != obj_list.end(); ++it)
  {
    sc_object *obj = *it;
    print_node (o, *obj);
  }
}

bool
module_hierarchy2dot (const char *name)
{
	if (!name)
		return false;
	string filename;
	filename =  name;
	filename += ".dot";
	ofstream o;
  o.open (filename.c_str(),ios::out | ios::trunc);
	if (o.is_open () == false)
		return false;
	o << "strict digraph " << name << " {\n";
  o << "node [shape=box];\n";
  print_edge (o);
	print_node (o,sc_get_top_level_objects ());
	o << "}\n";
	o.close ();
	return true;
}

} // end of sc_core namespace

