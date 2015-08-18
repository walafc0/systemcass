/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :           methodprocess_dependency.cc             |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   22_11_2005                      |
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
#include <iostream>
#include <fstream>

#include "methodprocess_dependency.h"
#include "simplify_string.h"
#include "sc_module.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;
namespace sc_core {

string get_name(const method_process_t * method) {
    assert(method != NULL);
    const sc_module *module = method->module;
    assert(module != NULL);
    const char * module_name = module->name();
    const char * function_name = method->name;

    string name = module_name;
    name += "_";
    name += function_name;
    return name;
}


static bool dot_write(ofstream & o, const SignalDependencyGraph & sig) {
    // Preparing data
    typedef map < const equi_t *, const method_process_t *>table_t;
    table_t table;

    // For each arrow, add destination node into table
    SignalDependencyGraph::const_iterator sig_it;
    for (sig_it = sig.begin(); sig_it != sig.end(); ++sig_it) {
        const SignalDependency & sd = *sig_it;
        const equi_t * equi = sd.destination;
        const method_process_t * method = sd.method;
        table[equi] = method;
    }

    // Printing into dot file
    string s;
    SignalDependencyGraph::const_iterator it;
    for (it = sig.begin(); it != sig.end(); ++it) {
        const SignalDependency & sd = *it;
        const equi_t * source_equi = sd.source;
        assert(source_equi != NULL);
        const method_process_t * source_method = table[source_equi];
        if (source_method == NULL) {
            continue;
        }
        string source_method_name = get_name(source_method);
        string source_equi_name = get_name(*source_equi);
        const method_process_t *destination_method = sd.method;
        string destination_method_name = get_name(destination_method);
        o << "edge [label=" << simplify_name(source_equi_name.c_str(), s) << "];\n";
        o << simplify_name(source_method_name.c_str(), s);
        o << " -> ";
        o << simplify_name(destination_method_name.c_str(), s);
        o << ";\n";
    }
}


bool MethodProcessDependencyGraph2dot(const char * name, const SignalDependencyGraph & sig) {
    if (!name) {
        return false;
    }
    string filename;
    filename = name;
    filename += ".dot";
    ofstream o;
    o.open(filename.c_str(), ios::out | ios::trunc);
    if (o.is_open() == false) {
        return false;
    }
    o << "strict digraph " << name << " {\n";
    dot_write(o, sig);
    // Closing dot file
    o << "}\n";
    o.close();
    if (dump_stage) {
        cerr << "MethodProcess dependency graph written into '" << filename << "'.\n";
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
