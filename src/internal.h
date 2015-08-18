/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   internal.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include "internal_ext.h"
#include <map>
#include <string>
#include <vector>

namespace sc_core {

// Method Process List
extern method_process_t * method;
extern method_process_list_t method_process_list;

// Module Naming Stack
typedef std::vector<std::string> module_name_stack_t;
extern module_name_stack_t module_name_stack;

// Hash Table Port -> Module
typedef std::map</*const */sc_port_base *, const sc_module *> port2module_t;
extern port2module_t port2module;

// Functions for Elaboration step
void sort_equi_list();
void create_signals_table();
void bind_to_table();

// Debug Functions
void print_table(std::ostream&);
void print_table_stats(std::ostream&);
void print_registers_writing_stats(std::ostream&);

extern bool is_clock(const sc_interface &inter);

// Flags
extern bool check_port_dependencies;
extern bool dump_all_graph;
extern const char * dump_module_hierarchy;
extern bool dump_netlist_info;
extern bool dump_funclist_info;
extern bool dynamic_link_of_scheduling_code;
extern bool keep_generated_code;
extern bool nosimulation;
extern bool notrace;
extern bool print_user_resources;
extern char * save_on_exit;
extern int scheduling_method;
extern bool use_port_dependency;
extern bool use_openmp;

#define NO_SCHEDULING       0
#define BUCHMANN_SCHEDULING 1
#define MOUCHARD_SCHEDULING 2
#define CASS_SCHEDULING     4

// More

extern uint64 trace_start;

}

#endif

