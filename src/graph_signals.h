/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 graph_signals.h                   |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   22_09_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __GRAPH_SIGNALS_H__
#define __GRAPH_SIGNALS_H__

#include "graph.h"
#include "signal_dependency.h"

/* A unique exported function from all that */
namespace sc_core {

extern Graph * makegraph(const SignalDependencyGraph&);

} // end of sc_core namespace

#endif /* __GRAPH_SIGNALS_H__ */

