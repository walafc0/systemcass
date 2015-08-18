/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 process_dependency.h              |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef MODULE_DEPENDENCY_H
#define MODULE_DEPENDENCY_H

#include "signal_dependency.h"

namespace sc_core {

// Signal Dependency Graph
struct ProcessDependency {
    const method_process_t * source;
    const method_process_t * destination;
    bool operator < (const ProcessDependency &) const;
};

typedef std::set<ProcessDependency> ProcessDependencyGraph;
typedef std::list<const method_process_t *> ProcessDependencyList;

// Convert Port Dependency Graph to Signal Dependency Graph
extern ProcessDependencyGraph * MakeProcessDependencyGraph(const SignalDependencyGraph &);
extern ProcessDependencyList * MakeProcessDependencyList(const SignalDependencyGraph &);

// Dump to dot file
extern bool ProcessDependencyGraph2dot(const char * name, const ProcessDependencyGraph &);
extern bool ProcessDependencyList2dot(const char * name, const ProcessDependencyList &);

} // end of sc_core namespace

#endif

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

