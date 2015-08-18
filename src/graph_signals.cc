/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 graph_signals.cc                  |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   22_09_2004                      |
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
 * $Log: graph_signals.cc,v $
 * Revision 1.7  2006/06/12 14:02:04  buchmann
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
 * Revision 1.6  2005/10/12 10:01:00  buchmann
 * Bug fix :
 * - test to avoid malloc(0)
 *
 * Remove :
 * - unused variable
 *
 * Revision 1.5  2005/09/14 14:08:24  buchmann
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
 * Revision 1.4  2005/06/22 09:15:03  buchmann
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
 * Revision 1.3  2005/03/25 14:33:01  buchmann
 * Typo :
 * -  dependAncy -> dependEncy
 *
 * sc_initialize :
 * -  Use a hash table to speed up elaboration step. (x40 faster)
 *
 * Tracing :
 * -  check for modification BEFORE building bit string.
 * -  use sprintf instead std string concatenation.
 *
 * Revision 1.2  2005/01/20 09:15:12  buchmann
 * add following functions to sc_uint classes :
 * - operator []
 * - range (left,right)
 *
 * support to port dependancy declarations.
 * print used precompiled options in verbose mode.
 * use pedantic flag.
 * add some rules to generate documentations.
 *
 * Revision 1.1  2004/09/27 14:40:14  buchmann
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

#include <cstdio>
#include <map>

#include "graph_signals.h"
#include "signal_dependency.h"
#include "sc_sensitive.h"
#include "sc_module.h"
#include "sc_port.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {


typedef std::map < const equi_t *, Vertex * >nodes_set_t;

void create_arcs(const SignalDependencyGraph & sig_g, nodes_set_t & s) {
   SignalDependencyGraph::const_iterator j;
   for (j = sig_g.begin(); j != sig_g.end(); ++j) {
      new_arc(s[j->source], s[j->destination]);
   }
}


void create_vertices_list(Graph * g, nodes_set_t & s) {
   Vertex * v = g->vertices;
   if (v == NULL) {
      return;
   }
   nodes_set_t::iterator j;
   for (j = s.begin(); j != s.end(); ++j) {
      v->data = (void *) (j->first);
      j->second = v++;
   }
}


nodes_set_t * create_nodes_set(const SignalDependencyGraph & sig_g) {
   /* Create a signal set */
   nodes_set_t & s = *(new nodes_set_t);

   SignalDependencyGraph::const_iterator j;
   for (j = sig_g.begin(); j != sig_g.end(); ++j) {
      s[j->source] = NULL;
      s[j->destination] = NULL;
   }
   return &s;
}


/* Construit le graph de dépendance des signaux de Mealy */
/* g = Mealy signal dependancy graph */
Graph * makegraph(const SignalDependencyGraph & sig_g) {
   int n = 0; /* Number of vertices */
   Graph * g;

   /* Create node set */
   nodes_set_t * sig_s = create_nodes_set(sig_g);

   /* Compute the number of vertices in the graph */
   n = sig_s->size();

   /* Create the graph */
   g = gb_new_graph(n);

   /* Create the node set */
   create_vertices_list(g, *sig_s);

   /* initialisation des vertices */
   create_arcs(sig_g, *sig_s);

   delete sig_s;

   return g;
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


