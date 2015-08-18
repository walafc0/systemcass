/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 graph.h                           |
  |                                                             |
  | Author  :                 Pétrot Frédéric                   |
  |                           Taktak Sami                       |
  |                           Buchmann Richard                  |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "sc_fwd.h"
#include <vector>

/* Graph.h: translation of a netlist into a Stanford Graphbase graph.
 * This structure is more natural than the bipartite graph for many
 * algorithms that consider the relationship between components
 * globally, not on a connectors per connector basis.
 * It is very much inspired from Don Knuth Stanford Graph Base, but 
 * is a real simplification. */

enum vertex_type {
    CONNECTOR = 0x0, INSTANCE = 0x4, STRONG_COMPONENT = 0x8
};

//#define KEEP_ARC 240898

typedef union {
    struct vertex_struct * V;
    struct arc_struct * A;
    struct graph_struct * G;
    char * S;
    long I;
} util;

typedef struct vertex_struct {
    struct arc_struct * arcs;
    union {
        void * data;
    };
    util u, v, w, x, y, z;
} Vertex;

typedef struct arc_struct {
    struct vertex_struct * tip;
    struct arc_struct * next;
    long len;
    util a, b;
} Arc;

//#define ID_FIELD_SIZE 161
typedef struct graph_struct {
    Vertex * vertices;
    long n;
    long m;
    char * id;
    util uu, vv, ww, xx, yy, zz;
} Graph;

extern Graph * gb_new_graph(int n);
extern void new_arc(Vertex * u, Vertex * v);

typedef std::vector<void *> component_list_t;
typedef std::vector<component_list_t *> strong_component_list_t;
extern strong_component_list_t * strong_component(Graph * g);

extern bool has_cycle(const strong_component_list_t &);

#endif /* __GRAPH_H__ */

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

