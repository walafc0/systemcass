/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :           methodprocess_dependency.h              |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   22_11_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef METHODPROCESS_DEPENDENCY_H
#define METHODPROCESS_DEPENDENCY_H

#include "signal_dependency.h"

namespace sc_core {

// Dump to dot file
extern bool MethodProcessDependencyGraph2dot (const char *name, const SignalDependencyGraph&);

}

#endif

