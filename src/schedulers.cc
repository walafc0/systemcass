/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 schedulers.cc                     |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Nicolas Pouillon                  |
|                                                             |
| Date    :                   23_03_2007                      |
|                                                             |
\------------------------------------------------------------*/

/* 
 * This file is part of the Disydent Project
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/disydent
 * E-mail             : mailto:richard.buchmann@lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Disydent is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <cstdio>
#include <cassert>
#include <iostream>
#include <algorithm> //std::sort

#include "sc_module.h" // method_process_t
#include "gen_code.h"  // gen_scheduling_code_for_dynamic_link & gen_scheduling_code_for_static_func
#include "internal.h"  // dump_all_graph
#include "graph_cass.h" // makegraph
#include "process_dependency.h" // MakeProcessDependencyList
#include "signal_dependency.h" // MakeSignalDependencyGraph
#include "mouchard_scheduling.h" // MakeMouchardScheduling
#include "graph_signals.h" // makegraph

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

/* ***************************** */
/* Dumping functions (for debug) */
/* ***************************** */

template <typename T>
ostream & operator << (ostream & o, const vector<T*> & v) {
    typename vector<T *>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i) {
        o << **i << " ";
    }
    return o;
}

/* ************ */
/*  functions   */
/****************/

// sort_functions splits and sorts instances_list into three functions lists : 
method_process_list_t transition_func_list;
method_process_list_t moore_func_list;
method_process_list_t combinational_func_list;


static bool sort_by_module_ptr (const method_process_t * a1, const method_process_t * a2) {
    assert(a1 != NULL);
    assert(a2 != NULL);
    return (a1->module < a2->module);
}


static bool sort_by_fct_ptr (const method_process_t * a1, const method_process_t * a2) {
    assert(a1 != NULL);
    assert(a2 != NULL);
    union {
        SC_ENTRY_FUNC func;
        unsigned long addr_l;
        unsigned long long addr_ll;
    } addr1, addr2;

    addr1.func = a1->func;
    addr2.func = a2->func;
    
    if (a1->func == a2->func) {
        return sort_by_module_ptr(a1,a2);
    }

    if (sizeof(SC_ENTRY_FUNC) == 4 ) {
        return (addr1.addr_l < addr2.addr_l);
    }
    else {
        return (addr1.addr_ll < addr2.addr_ll);
    }
}



void sort_functions() {
    method_process_list_t::const_iterator m;
    for (m = method_process_list.begin(); m != method_process_list.end(); ++m) {
        if ((*m)->is_combinational()) {
            combinational_func_list.push_back(*m);
        }
        else if ((*m)->is_transition ()) {
            transition_func_list.push_back(*m);
        }
        else if ((*m)->is_genmoore ()) {
            moore_func_list.push_back(*m);
        }
    }
    // Sort transition functions by method pointer (1) and by module pointer (2)
    std::sort (transition_func_list.begin(), transition_func_list.end(), sort_by_fct_ptr);
    // Sort generation functions by method pointer (1) and by module pointer (2)
    std::sort (moore_func_list.begin(), moore_func_list.end(), sort_by_fct_ptr);
}

/* ****************** */
/* process dependency */
/* ****************** */

static SignalDependencyGraph * MakeAcyclicSignalDependencyGraph() {
    if (dump_all_graph) {
        const PortDependencyGraph & port_graph = get_port_dependency_graph ();
        PortDependencyGraph2dot ("port_graph", port_graph);
    }

    SignalDependencyGraph * sig_graph = MakeSignalDependencyGraph();

    if (dump_all_graph) {
        SignalDependencyGraph2dot ("signal_graph",*sig_graph);
    }

    if (!Check(*sig_graph)) {
        cerr << "The signal dependency graph is not valid.\n";
        exit (29092004);
    }

    if (!Check(method_process_list, *sig_graph)) {
        cerr << "Sensitivity list is not valid.\n";
        exit (30092004);
    }

    // There is a cycle in the signal dependency graph ?
    Graph * sig_knuth = makegraph(*sig_graph);
    strong_component_list_t * s = strong_component(sig_knuth);

    if (dump_all_graph) {
        SignalDependencyOrder2txt("signal_order",*s);
    }

    if (has_cycle(*s)) {
        cerr << "Error : There is a cycle in the signal dependency graph.\n";
        exit (24092004);
    }
    return sig_graph;
}


static ProcessDependencyList * MouchardScheduling() {
    SignalDependencyGraph * sig_graph = MakeAcyclicSignalDependencyGraph();
    assert(sig_graph != NULL);
    // Create the process evaluation list
    ProcessDependencyList * process_list = MakeMouchardScheduling(*sig_graph);
    assert(process_list != NULL);

    if (dump_all_graph) {
        ProcessDependencyList2dot  ("process_order", *process_list);
    }

    return process_list;
}


static ProcessDependencyList * BuchmannScheduling() {
    SignalDependencyGraph * sig_graph = MakeAcyclicSignalDependencyGraph();
    // Create the process evaluation list
    ProcessDependencyList * process_list = MakeProcessDependencyList(*sig_graph);

    if (dump_all_graph) {
        ProcessDependencyList2dot("process_order", *process_list);
    }

    return process_list;
}


string get_scheduling(int scheduling_method) {
    /* marque les fonctions comme fonction de mealy ou non */
    if (dump_funclist_info) {
        cerr << "method process list : " << method_process_list << "\n";
    }

    sort_functions();
    if (dump_funclist_info) {
        cerr << "Transition functions : " << transition_func_list << "\n";
        cerr << "Moore generation functions : " << moore_func_list << "\n";
        cerr << "Mealy generation functions : " << combinational_func_list << "\n";
    }

    /* Schedule */ 
    string base_name;
    switch (scheduling_method) {
        case BUCHMANN_SCHEDULING :
        {
            // Generate the scheduled code, compile and link.
            // Buchmann's thesis explains this scheduling method.
            // Uses port dependancies like Dr. Mouchard.
            ProcessDependencyList * process_list = BuchmannScheduling();
            if (dynamic_link_of_scheduling_code) {
                base_name = gen_scheduling_code_for_dynamic_link(transition_func_list, moore_func_list, *process_list);
            }
            else {
                gen_scheduling_code_for_static_func(transition_func_list, moore_func_list, *process_list);
            }
            break;
        }

        case MOUCHARD_SCHEDULING :
        {
            // Generate the scheduled code, compile and link.
            // Mouchard's thesis explains this scheduling method.
            // Uses port dependancies like Dr. Mouchard.
            // CAUTION : unlike FastSysC, this scheduling is totally static 
            // and does not use an event-driven scheduler.
            ProcessDependencyList * process_list = MouchardScheduling();
            if (dynamic_link_of_scheduling_code) {
                base_name = gen_scheduling_code_for_dynamic_link(transition_func_list, moore_func_list, *process_list);
            }
            else {
                gen_scheduling_code_for_static_func (transition_func_list, moore_func_list, *process_list);
            }
            break;
        }

        case CASS_SCHEDULING :
        {
            // Generate the scheduled code, compile and link
            // Hommais's thesis explains this scheduling method (like CASS strategy)
            // Doesn't use port dependancies
            Graph * g = makegraph (&combinational_func_list);
            if (dump_all_graph && g) {
                graph2dot("module_graph", *g);
            }
            strong_component_list_t * strong_list = strong_component(g);
            if (dynamic_link_of_scheduling_code) {
                base_name = gen_scheduling_code_for_dynamic_link(transition_func_list, moore_func_list, *strong_list);
            }
            else {
                gen_scheduling_code_for_quasistatic_func (transition_func_list, moore_func_list, *strong_list);
            }
            break;
        }
        default :
            cerr << "Error : Unable to schedule SystemC process."
                    "Please select a scheduling method.\n";
            exit (35);
    }
    return base_name;
}


bool run_schedule_editor (const char * schedule) {
    char buf[128];
    const char * editor = getenv("EDITOR");
    if (editor == NULL) {
        editor = "vim";
    }
    sprintf (buf, "(cd '%s' ; %s '%s')", temporary_dir, editor, schedule);
    if (dump_stage) {
        cerr << "Executing : " << buf << endl;
    }
    return (system(buf) == 0);
}


} // end of sc_core namespace

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

