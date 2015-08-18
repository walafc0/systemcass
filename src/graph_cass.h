/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 graph_cass.h                      |
|                                                             |
| Author  :                 Pétrot Frédéric                   |
|                           Taktak Sami                       |
|                           Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __GRAPH_CASS_H__
#define __GRAPH_CASS_H__

#include "graph.h"

namespace sc_core {

/* A unique exported function from all that */
extern Graph * makegraph(const method_process_list_t *);
extern bool graph2dot(const char *, const Graph&);

} // end of sc_core namespace

#endif /* __GRAPH_CASS_H__ */

