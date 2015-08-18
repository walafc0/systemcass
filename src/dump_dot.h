/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  dump_dot.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef DUMP_DOT_H
#define DUMP_DOT_H

#include <list>

#include "sc_fwd.h"
#include "graph.h"

namespace sc_core {

extern bool SignalDependancyGraph2dot(const char * name, method_process_list_t & lm);
extern bool FctDependancyGraph2dot(const char * name, Graph *);

}

#endif

