/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  port_dependency.h                |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef PORT_DEPENDENCY_H
#define PORT_DEPENDENCY_H

#include "sc_fwd.h"
#include <list>

#include "port_dependency_ext.h"

namespace sc_core {

// Port Dependency Graph
struct PortDependency {
  const method_process_t * method;
  const sc_port_base * source;
  const sc_port_base * destination;
};

typedef std::list<PortDependency> PortDependencyGraph;

// Accessor
extern const PortDependencyGraph& get_port_dependency_graph ();

// Dump to dot file
extern bool PortDependencyGraph2dot(const char * name, const PortDependencyGraph& = get_port_dependency_graph());

} // end of sc_core namespace

#endif

