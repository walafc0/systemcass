/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 graph.cc                          |
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
 * $Log: graph.cc,v $
 * Revision 1.10  2006/06/12 14:02:04  buchmann
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
 * Revision 1.9  2005/10/12 10:00:59  buchmann
 * Bug fix :
 * - test to avoid malloc(0)
 *
 * Remove :
 * - unused variable
 *
 * Revision 1.8  2005/09/14 14:08:24  buchmann
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
 * Revision 1.7  2005/03/29 14:04:30  buchmann
 * Bug fix :
 * -  Evaluation order of Mealy functions
 *
 * Print more informations when DUMP_STAGE is defined.
 *
 * Revision 1.6  2005/01/20 09:15:12  buchmann
 * add following functions to sc_uint classes :
 * - operator []
 * - range (left,right)
 *
 * support to port dependancy declarations.
 * print used precompiled options in verbose mode.
 * use pedantic flag.
 * add some rules to generate documentations.
 *
 * Revision 1.5  2004/09/27 14:40:13  buchmann
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
#include <cstring>
#include <map>

#include "graph.h"
#include "sc_sensitive.h"
#include "sc_module.h"
#include "sc_port.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

/* Sorry for that, Mr Knuth :-) */

Graph * gb_new_graph(int n) {
    Graph * g;

    g = (Graph *) malloc(sizeof(Graph));
    g->n = n;
    if (n) {
        g->vertices = (Vertex *) malloc(n * sizeof(Vertex));
        (void) memset(g->vertices, 0, n * sizeof(Vertex));
    }
    else {
        g->vertices = NULL;
    }
    return g;
}


static void gb_new_arc(Vertex * u, Vertex * v, long l) {
    Arc * a;
    a = (Arc *) malloc(sizeof(Arc));
    a->next = u->arcs;
    u->arcs = a;
    a->tip = v;
    a->len = l;
}

/* end Sorry */

#define VERTEX 220898

#define type u.I

void new_arc(Vertex * u, Vertex * v) {
    Arc * a;
#ifdef CONFIG_DEBUG
    if ((u == NULL) || (v == NULL)) {
        exit(29042004);
    }
#endif
    for (a = u->arcs; a != NULL; a = a->next) {
        if (a->tip == v) {
            return;
        }
    }
    gb_new_arc(u, v, 0L);
}


#define rank z.I
#define parent y.V
#define untagged x.A
#define link w.V
#define min v.V

#define infinity g->n

/* strong_component: extracts and topologically sorts the strong components of
 * a graph.
 * The returned data structure is a chain_list of chain_lists.
 * The first list implicitely lists the components in order, and the pointed
 * to ones list the components contents (one or more vertices) */

extern strong_component_list_t *strong_component(Graph * g) {
    long nn;
    Vertex * active_stack;
    Vertex * settled_stack;
    Vertex * vv;

    register Vertex * v;

    strong_component_list_t * strongcomponents = new strong_component_list_t;
    typedef std::vector < void *>void_list_t;
    void_list_t * component;

    if (g->vertices == NULL) {
        return strongcomponents;
    }

    /* Make all vertices unseen and all arcs untagged */
    for (v = g->vertices + g->n - 1; v >= g->vertices; v--) {
        v->rank = 0;
        v->untagged = v->arcs;
    }
    nn = 0;
    active_stack = settled_stack = NULL;

    for (vv = g->vertices; vv < g->vertices + g->n; vv++) {
        if (vv->rank == 0) { /* vv is still unseen */
            v = vv;
            v->parent = NULL;

            v->rank = ++nn;
            v->link = active_stack;
            active_stack = v;
            v->min = v;

            do { /* Eplore one step from the current vertex v, possibly moving to another vertex 
                           and calling it v */
                register Vertex *u; /* a vertex adjacent to v */
                register Arc *a = v->untagged; /* v's first remaining untagged arc, if any */
                if (a) {
                    u = a->tip;
                    v->untagged = a->next; /* tag the arc from v to u */
                    if (u->rank) { /* we've seen u already */
                        if (u->rank < v->min->rank) { /* non-tree arc, jutt update v->min */
                            v->min = u;
                        }
                    }
                    else { /* u is presently unseen */
                        u->parent = v; /* the arcfrom v to u is a new tree arc */
                        v = u; /* u will now be the currnt vertex */
                        v->rank = ++nn; /* make vertex v active */
                        v->link = active_stack;
                        active_stack = v;
                        v->min = v;
                    }
                }
                else { /* all arcs from v are tagged, so v matures */
                    u = v->parent; /* prepare to backtrack in the tree */
                    if (v->min == v) {
                        /* Remove v and all its successors on the activestack from the tree,
                           and mark them as a strong component of the graph */
                        register Vertex * t; /* runs though the vertices of the new strong component */

                        t = active_stack;
                        active_stack = v->link;
                        v->link = settled_stack;
                        settled_stack = t; /* we've moved the top of one stack to the other */
                        component = new void_list_t;
                        /* singe vetex */
                        if (t != v) { /* NOT singe vetex */
                            while (t != v) {
                                component->push_back(t->data);
                                t->rank = infinity; /* now t is settled */
                                t->parent = v; /* and v represents the new strong component */
                                t = t->link;
                            }
                        }
                        component->push_back(v->data);
                        strongcomponents->push_back(component);
                        v->rank = infinity; /* v too is settled */
                        v->parent = v; /* and represents its own strong component */
                    }
                    else {
                        /* the arc from u to v has just matured, making v->min visible from u */
                        if (v->min->rank < u->min->rank) {
                            u->min = v->min;
                        }
                    }
                    v = u;/* the former parent of v is the new current vertex v */
                }
            } while (v != NULL);
        }
    }

    return strongcomponents;
}


bool has_cycle(const strong_component_list_t & l) {
    strong_component_list_t::const_iterator it;
    for (it = l.begin(); it != l.end(); ++it) {
        if ((*it)->size() > 1) {
            return true;
        }
    }
    return false;
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

