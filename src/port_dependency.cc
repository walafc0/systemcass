/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                  port_dependency.cc               |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   21_09_2004                      |
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

#include <cstring>
#include <list>
#include <iostream>
#include <fstream>

#include "port_dependency.h"
#include "simplify_string.h"
#include "sc_fwd.h"
#include "sc_object.h"
#include "sc_module.h"
#include "sc_port.h"
#include "sc_ver_ext.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

static PortDependencyGraph aPortDependencyGraph;

const PortDependencyGraph & get_port_dependency_graph() {
    return aPortDependencyGraph;
}


static void check_port_dependency_declaration(const sc_port_base * a, const sc_port_base& b) {
    const sc_module & moduleA = a->get_module();
    const sc_module & moduleB = b.get_module();
    if (check_port_dependencies && (&moduleA != &moduleB)) {
        const char * module_nameA = moduleA.basename();
        const char * module_nameB = moduleB.basename();
        if (strlen(module_nameA) == 0) {
            module_nameA = "<top level>";
        }
        cerr << "Error : Port dependency declaration in '"
            << module_name_stack.back () << "' constructor is wrong.\n" 
            << "'" << a->name () << "' and '" << b.name ()
            << "' are ports of two differents modules ("
            << module_nameA << " and " 
            << module_nameB << "). "
            << "You can't declare any port dependency like that.\n";
        exit (37); //15092005);
    }
    if (module_name_stack.empty()) {
        cerr << "Error : Port dependency declaration is wrong.\n" 
            << "The dependency declaration of '" 
            << a->name () << "' and '" << b.name ()
            << "' is not in any sc_module constructor.\n";
        exit (16092005);
    }
    if (method_process_list.empty()) {
        cerr << "Error : Port dependency declaration in '"
            << module_name_stack.back () << "' constructor is wrong.\n" 
            << "The dependency of '" << a->name () << "' and '" << b.name ()
            << "' should be linked to a SC_METHOD.\n";         
        exit(165); //17092005);
    }
}


void set_port_dependency(const sc_port_base * a, const sc_port_base & b) {
    check_port_dependency_declaration(a,b);
    PortDependency p;
    p.method = method_process_list.back();
    p.source = a;
    p.destination = &b;
    aPortDependencyGraph.push_back(p);
#ifdef DUMP_PORT_DEPENDENCY
    if (a) {
        cerr << "'" << ((sc_object&)b).name()
            << "' depends on '" 
            << ((sc_object*)a)->name() 
            << "' in '" << p.method->name << "' function.\n";
    }
    else {
        cerr << "'" << ((sc_object&)b).name()
            << "' doesn't depend on anything"
            << " in '" << p.method->name << "' function.\n";
    }
#endif
}


static void dot_write (ofstream & o, const PortDependencyGraph & g) {
    string s;
    PortDependencyGraph::const_iterator it;
    for (it = g.begin (); it != g.end (); ++it) {
        if (it->source == NULL) {
            continue;
        }
        const char * name;
        name = ((sc_object *)it->source)->name();
        o << simplify_name(name,s);
        o << " -> ";
        name = ((sc_object *)it->destination)->name();
        o << simplify_name(name,s);
        o << "\n";
    }
}


bool PortDependencyGraph2dot (const char * name, const PortDependencyGraph & g) {
    if (!name) {
        return false;
    }
    string filename;
    filename = name;
    filename += ".dot";
    ofstream o;
    o.open(filename.c_str(),ios::out | ios::trunc);
    if (o.is_open () == false) {
        return false;
    }
    o << "// Port dependency graph\n"
        "// Generated by "
        << sc_version () << "\n";
    o << "strict digraph " << name << " {\n";
    dot_write (o,g);
    o << "}\n";
    o.close ();
    if (dump_stage) {
        cerr << "Port Dependency Graph written into '" << filename << "'.\n";
    }
    return true;
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

