/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 mouchard_scheduling.h             |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   20_12_2006                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef MOUCHARD_SCHEDULING_H
#define MOUCHARD_SCHEDULING_H

#include "process_dependency.h"

namespace sc_core {

ProcessDependencyList * MakeMouchardScheduling(const SignalDependencyGraph &);

} // end of sc_core namespace

#endif

