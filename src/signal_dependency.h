/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  signal_dependancy.h              |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef SIGNAL_DEPENDENCY_H
#define SIGNAL_DEPENDENCY_H

#include <set>

#include "sc_fwd.h"
#include "entity.h" // equi_t
#include "graph.h"
#include "port_dependency.h"

namespace sc_core {


// Signal Dependency Graph
struct SignalDependency {
  const method_process_t * method;
  const equi_t * source;
  const equi_t * destination;
  bool operator < (const SignalDependency & a) const;
};


typedef std::set<SignalDependency> SignalDependencyGraph;


// Convert Port Dependency Graph to Signal Dependency Graph
extern SignalDependencyGraph * MakeSignalDependencyGraph(const PortDependencyGraph & = get_port_dependency_graph());

// Checking
extern bool Check(const SignalDependencyGraph &);
extern bool Check(const method_process_list_t &, const SignalDependencyGraph &);

// Dump
extern bool SignalDependencyGraph2dot(const char * name, const SignalDependencyGraph &);
extern bool SignalDependencyOrder2txt(const char * name, const strong_component_list_t &);

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

