/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 graph_cass.cc                     |
  |                                                             |
  | Author  :                 Petrot Frédéric                   |
  |                           Taktak Sami                       |
  |                           Buchmann Richard                  |
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

/*
 * $Log: graph_cass.cc,v $
 * Revision 1.5  2006/06/12 14:02:04  buchmann
 * - Now sort transition and generation functions by fonction pointer
 *   (Thank to Nicolas Pouillon)
 * - Add Nicolas Pouillon as contributor into splash screen
 * - Rename files and classes from "dependancy" to "dependency".
 * - Add some references to bibliography file
 * - Add licence note to every files
 * - Rename "module graph" to "process graph"
 * - Now dump module graph when using --t option and CASS scheduling at the same
 *   time
 *
 * Bug fixes :
 * - check user time ( != 0) before computing simulation performance
 * - Remove reinterpret_cast (for pending write) because of unexpected results
 * - Add ASSERT in trace functions
 *
 * Revision 1.4  2005/09/14 14:08:24  buchmann
 * Add Werror flag to compile the debug library.
 * Split sc_clock implementation from sc_port to sc_clock.
 * Add a helper message to write mealy functions avoiding combinational cycle.
 *
 * Bug fix :
 * - cvs rules is no longer circular
 * - bit2string
 * - port binding (sc_in to sc_out)
 * - error message from FSM rules checker
 * - sc_time copy operator
 *
 * Code cleaning :
 * - remove duplicated code (entity.cc and sc_port.cc)
 * - remove duplicated declarations (internal_ext.h and internal.h)
 *
 * Revision 1.3  2005/06/22 09:15:03  buchmann
 * Add some *_ext.h files to split internal implementation from API interface.
 *
 * Add -Wunused to detect unused functions.
 *
 * Include directory now contains API interface file only.
 *
 * Declar all the implementation/interface inside sc_core namespace as LRM 2.1
 * advices.
 *
 * Remove casc namespace.
 *
 * Clean up dead code segments.
 *
 * Add hexadecimal dumping support.
 *
 * Add empty implementation of functions for object hierarchy traversal. (LRM 2.1)
 *
 * Bug fixes :
 * - reference return of the following functions : operators =, |=, &= and so on.
 * - range functions now return an sc_int_subref instead of an int.
 *
 * Revision 1.2  2005/05/30 12:33:50  buchmann
 * Makefile :
 * - use ${...} form instead of $(...) one.
 * - add entity file object and entity, sc_signal headers.
 *
 * Split sc_signal(s)/sc_port(s) into two separate files.
 * Add some codes to check hex2string function.
 * Add some informations about -rdynamic when there is an error using dlopen.
 * Add some ASSERT(s) for debug library.
 * Add module name into the warning printing.
 * Minor changes for PAT traces.
 *
 * Revision 1.1  2004/09/27 14:40:13  buchmann
 * bug fix :
 * - allow the use of user-defined structs in sc_signal/sc_in/sc_out/sc_inout
* - use sc_dt namespace like official SystemC engine
*
* add :
* - dump the signal/port/module dependancy graph in dot format
*
* Graph library is now splitted from the SystemCASS specific code.
*
*/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "graph_cass.h"
#include "sc_sensitive.h"
#include "sc_module.h"
#include "sc_port.h"
#include "entity.h"
#include "simplify_string.h" // simplify_string
#include "sc_ver_ext.h" // sc_version for dumping to DOT
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

std::map < const method_process_t *, Vertex * >m_v;

/* Ajoute un arc */
void add_arcs(const method_process_t & mp, const sc_module & module) {
    std::map < const method_process_t *, Vertex * >::iterator i;
    for (i = m_v.begin(); i != m_v.end(); ++i) {
        if (i->first->module == &module)
            new_arc(m_v[&mp], i->second);
    }
}


/* Ajoute un arc */
void add_arcs(const method_process_t & mp, tab_t * signal_pointer) {
    equi_t & e = get_equi(signal_pointer);
    equi_t::iterator i;
    for (i = e.begin(); i != e.end(); ++i) {
        const char * kind = i->kind();
        if (strstr(kind, "out")) {
            const sc_port_base & port = *(i->port);
            const sc_module & module = port.get_module();
            if (&module == mp.module) {
                continue;
            }
            add_arcs(mp, module);
            break;
        }
    }
}


void add_arcs(const method_process_t & mp) {
    sensitivity_list_t::const_iterator port_bound;
    for (port_bound = mp.sensitivity_list.begin();
            port_bound != mp.sensitivity_list.end(); ++port_bound) {
        // pour tous les ports/signaux p associés à la fonction au couple m
        /* si pp est un port de sortie d'une autre fonction de mealy,
           crée un arc entre m et mm */
        /* on ne peut pas savoir si un port est la sortie d'une fonction de mealy. 
           on va donc chercher à savoir si c'est un port de sortie d'un automate de moore,
           sinon on considère que c'est une sortie de mealy.
           */
        add_arcs(mp, port_bound->get_interface().get_pointer());
    }
}


/* Construit le graph de dépendance des signaux de Mealy */
/* lm est la liste des fonctions de Mealy */
/* -----
   Graph
   -----
   node = module
   edge = dependency

   Vertex is an array of (method_process_t*)
   Edge is a couple Vertex->Vertex.
   */
extern Graph * makegraph(const method_process_list_t * lm) {
    method_process_list_t::const_iterator mp;

    int n = 0; /* Number of vertices */
    Graph * g;
    Vertex * v;

    /* Compute the number of vertices in the graph:
     * The vertices set is the method set. */

    /*  for (m = lm->begin(); m != lm->end(); m++)
        n++;*/
    n = lm->size();

    /* Create the graph */
    g = gb_new_graph(n);

    /* Associate the netlist elements to the graph vertices, and vice-versa */
    v = g->vertices;

    /* initialisation des vertices */
    for (mp = lm->begin(); mp != lm->end(); ++mp) {
        v->data = *mp;
        m_v[*mp] = v++;
    }

    for (mp = lm->begin(); mp != lm->end(); ++mp) {
        // pour tous couples m, instance module/function de la liste lm
        add_arcs(**mp);
    }

    return g;
}


static string & get_method_process_fullname(string & name, const method_process_t & mp) {
    sc_module * module = mp.module;
    name = module->name();
    name += "_";
    name += mp.name;
    return name;
}


static bool dot_write(ofstream & o, const Graph & g) {
    string name1, name2, s;
    // Printing into dot file
    for (int i = 0; i < g.n; ++i) { // Printing node list
        Vertex * v = g.vertices + i;
        method_process_t *mp = (method_process_t *) (v->data);
        get_method_process_fullname(name1, *mp);
        const char * pname1 = name1.c_str();
        o << "node [name=";
        o.width(35);
        o << simplify_name(pname1, s);
        o << "];\n";
    }
    for (int i = 0; i < g.n; ++i) { // Printing edge list
        Vertex * v1 = g.vertices + i;
        method_process_t *mp1 = (method_process_t *) (v1->data);
        get_method_process_fullname(name1, *mp1);
        const char * pname1 = name1.c_str();
        for (Arc * a = v1->arcs; a != NULL; a = a->next) {
            Vertex *v2 = a->tip;
            method_process_t * mp2 = (method_process_t *) (v2->data);
            get_method_process_fullname(name2, *mp2);
            const char * pname2 = name2.c_str();
            o.width(35);
            o << simplify_name(pname2, s);
            o << " -> ";
            o.width(35);
            o << simplify_name(pname1, s);
            o << ";\n";
        }
    }
    return true;
}

extern bool graph2dot(const char * name, const Graph & g) {
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
    o << "// Module dependency graph\n// Generated by " << sc_version() << "\n";
    o << "strict digraph " << name << " {\n";
    o << "node [shape=box];\n";
    //       "concentrate=true;\n";
    o << left;
    dot_write(o, g);
    // Closing dot file
    o << "}\n";
    o.close();
    if (dump_stage) {
        cerr << "Module dependency graph written into '" << filename << "'.\n";
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

