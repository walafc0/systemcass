/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 global_functions.cc               |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                           Nicolas Pouillon                  |
  |                                                             |
  | Date    :                   21_09_2005                      |
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
#include <iostream>
#include <dlfcn.h>

#include "schedulers.h"   // get_scheduling & run_schedule_editor
#include "sc_module.h"    // check_all_method_process
#include "gen_code.h"     // gen_scheduling_code_for_dynamic_link & gen_scheduling_code_for_static_func
#include "sc_clock_ext.h" // clock list
#include "usage.h"
#include "module_hierarchy2dot.h"
#include "assert.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

const char * temporary_dir = "/tmp";
const char * generated_files_dir = "./generated_by_systemcass";

static usage_t usage;

/* ***************************** */
/* Dumping functions (for debug) */
/* ***************************** */

template < typename T > ostream & operator <<(ostream & o, const vector < T * > & v) {
    typename vector < T * >::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i) {
        o << **i << " ";
    } return o;
}

/* ************ */
/*  clock list  */
/****************/

typedef std::set<const sc_clock *> clock_list_t;

ostream & operator <<(ostream & o, const clock_list_t & cl) {
    clock_list_t::const_iterator i;
    for (i = cl.begin(); i != cl.end(); ++i) {
        o << (*i)->name() << " ";
    }
    return o;
}


// clock list
void create_clock_list(clock_list_t & c, const equi_list_t & el) {
    equi_list_t::const_iterator i;
    for (i = el.begin(); i != el.end(); ++i) {
        equi_t::const_iterator j;
        for (j = i->begin(); j != i->end(); ++j) {
            if (j->kind() == sc_clock::kind_string)
                c.insert((const sc_clock *) j->object);
        }
    }
}

/* ************ */
/*  functions   */
/****************/

// function pointer to the simulation core (compiled and linked dynamically)
evaluation_fct_t func_simulate_1_cycle = NULL;
evaluation_fct_t func_combinationals = NULL;

/* ************ */
/* dynamic link */
/* ************ */

static void * handle = NULL;

static void link(const char * lib) {
    // chargement du code de simulate_1_cycle
    handle = dlopen(lib, RTLD_GLOBAL | RTLD_NOW);
    //handle = dlopen(lib, RTLD_LAZY | RTLD_GLOBAL | RTLD_NOW);

    if (handle == NULL) {
        const char * error = dlerror();
        if (error) {
            fprintf(stderr, "dlopen: %s\n", error);
        }
        fprintf(stderr, "Is there -rdynamic option into your command line ? If not, please do it.\n");
        exit(18);
    }

    union uni_fct_t {
        evaluation_fct_t fct_type;
        void * dl_pointer_type;
    };
    uni_fct_t temp;
    temp.dl_pointer_type = dlsym(handle, "initialize");
    if (temp.dl_pointer_type == NULL) {
        const char * error = dlerror();
        if (error) {
            fprintf(stderr, "dlsym: %s\n", error);
        }
        exit(19);
    }
    evaluation_fct_t func_initialize;
    func_initialize = temp.fct_type;
    func_initialize();

    temp.dl_pointer_type = dlsym(handle, "simulate_1_cycle");
    if (temp.dl_pointer_type == NULL) {
        const char * error = dlerror();
        if (error) {
            fprintf(stderr, "dlsym: %s\n", error);
            exit(20);
        }
        func_simulate_1_cycle = temp.fct_type;

        temp.dl_pointer_type = dlsym(handle, "mealy_generation");
        if (temp.dl_pointer_type == NULL) {
            const char * error = dlerror();
            if (error) {
                fprintf(stderr, "dlsym: %s\n", error);
            }
            exit(21);
        }
        func_combinationals = temp.fct_type;

        /*
         *
         */
        if (dump_stage) {
            cerr << "dynamic link done\n";
        }
    }
}


static void unlink() {
    if (handle) {
        if (dlclose(handle) != 0) {
            cerr << "Warning : dlclose returns an error.\n" << dlerror() << endl;
        }
        handle = NULL;
    }
}

/* ************ */
/* initializing */
/* ************ */

bool already_initialized = false;

static void use_static_func() {
    if (dump_stage) {
        cerr << "Using static functions to schedule SystemC processes.\n";
    }
    if (scheduling_method == CASS_SCHEDULING) {
        func_simulate_1_cycle = (evaluation_fct_t) sc_core::quasistatic_simulate_1_cycle;
        func_combinationals = (evaluation_fct_t) sc_core::quasistatic_mealy_generation;
    }
    else {
        func_simulate_1_cycle = (evaluation_fct_t) sc_core::static_simulate_1_cycle;
        func_combinationals = (evaluation_fct_t) sc_core::static_mealy_generation;
    }
}


static void compile_and_link(const char *base_name) {
    if (dump_stage) {
        cerr << "Using dynamically loaded functions to schedule SystemC processes.\n";
    }
    // compilation du code de simulate_1_cycle
    compile_code(base_name);

    char lib_absolutepath[256];
#if defined(CONFIG_OS_DARWIN)
    sprintf(lib_absolutepath, "/tmp/%s.so", base_name);
#elif defined(CONFIG_OS_LINUX)
    sprintf(lib_absolutepath, "/tmp/.libs/%s.so.0", base_name);
#else
    cerr << "ERROR\n";
    exit(126);
#endif
    link(lib_absolutepath);
}


static void internal_sc_initialize(void) {
    sort_equi_list();

    check_all_method_process();

    if (dump_netlist_info) {
        cerr << "Module instance list\n--------------------\n" << instances_set << endl;
    }

    /*
     * Initialize the signals table
     */
    create_signals_table();
    bind_to_table();
    if (dump_netlist_info) {
        print_table(cerr);
        cerr << endl;
        print_table_stats(cerr);
        cerr << endl;
    }
    // Init variables to be able to run combinational functions
    pending_write_vector_capacity = get_signal_table_size();

    if (pending_write_vector_capacity == 0) {
        pending_write_vector = NULL;
    }
    else {
        pending_write_vector = (pending_write_vector_t) realloc(pending_write_vector, sizeof(pending_write_t) * pending_write_vector_capacity);
    }

    // create the clock list
    clock_list_t clock_list;
    create_clock_list(clock_list, get_equi_list());
    if (dump_netlist_info) {
        cerr << "Clock list\n" << "----------\n" << clock_list << "\n\n";
    }

    // Check if a clock exists in the system
    if (clock_list.empty()) {
        cerr << "System need a clock.\n" <<
            "Please define system clock using special type \"sc_clock\".\n";
        exit(22);
    }
    // Check if any constructor wrote into registers
    if (pending_write_vector_nb != 0) {
        cerr <<
            "Error : Register/Signal writing is not allowed before sc_initialize.\n"
            "Move initializations from constructors/sc_main to module reset sequences.\n";
        // we are unable to dump register(s) name(s) 
        // because the table binding is not yet completed.
        exit(24);
    }

    string base_name = get_scheduling(scheduling_method);

    if (dynamic_link_of_scheduling_code) {
        compile_and_link(base_name.c_str());
    }
    else {
        use_static_func();
    }

    pending_write_vector_nb = 0;

    check_all_ports();
    usage.start();

    if (dump_stage) {
        cerr << "sc_initialize () done.\n";
    }

    already_initialized = true;
}


inline void check_and_initialize() {
    if (already_initialized == false) {
#if defined(SYSTEMC_VERSION_1_0)
        cerr << "Warning : call sc_initialize before executiong simulation.\n";
#endif
        internal_sc_initialize();
        if (dump_module_hierarchy) {
            module_hierarchy2dot(dump_module_hierarchy);
        }
        if (nosimulation) {
            exit(0);
        }
    }
}


void sc_initialize(void) {
    cerr << "Warning : 'sc_initialize' function is deprecated since SystemC 2.1.\n";
    cerr << "Use 'sc_start(0)' instead.\n";
    check_and_initialize();
}

/* ********** */
/* simulating */
/* ********** */

inline void sc_cycle(double duration) {
    check_and_initialize();
    sc_core::internal_sc_cycle0(duration);
}


inline void sc_cycle(double duration, sc_time_unit time_unit) {
    check_and_initialize();
    sc_core::internal_sc_cycle0(duration);
}


void sc_start(double d_val) {
    sc_cycle(d_val);
#ifdef DUMP_SIGNAL_STATS
    print_registers_writing_stats(cerr);
#endif
#ifdef DUMP_SCHEDULE_STATS
    print_schedule_stats(cerr);
#endif
}


void sc_start() {
    sc_cycle(-1);
#ifdef DUMP_SIGNAL_STATS
    print_registers_writing_stats(cerr);
#endif
#ifdef DUMP_SCHEDULE_STATS
    print_schedule_stats(cerr);
#endif
}


void sc_start(double d_val, sc_time_unit d_tu) {
    sc_start(sc_time(d_val, d_tu));
}


void sc_start(const sc_time & duration) {
    sc_cycle((double) duration);
#ifdef DUMP_SIGNAL_STATS
    print_registers_writing_stats(cerr);
#endif
#ifdef DUMP_SCHEDULE_STATS
    print_schedule_stats(cerr);
#endif
}


/* ****************** */
/* stoping simulation */
/* ****************** */

bool have_to_stop = false;
sc_stop_mode stop_mode = SC_STOP_FINISH_DELTA;

void sc_stop() {
    switch (stop_mode) {
        case SC_STOP_IMMEDIATE:
            exit(54);
            break;
        case SC_STOP_FINISH_DELTA:
        default:
            have_to_stop = true;
            break;
    }
}


void sc_set_stop_mode(sc_stop_mode new_mode) {
    if (new_mode == SC_STOP_IMMEDIATE) {
        stop_mode = SC_STOP_IMMEDIATE;
    }
}


sc_stop_mode sc_get_stop_mode() {
    return stop_mode;
}


void close_systemcass() {
    unlink();
    if (print_user_resources) {
        usage.stop();
        unsigned int d = usage;
        cerr << "Performances\n" "------------\n";
        cerr << "Time elapsed (sec) : " << d << endl;
        cerr << "Cycles done        : " << nb_cycles << endl;
        if (d == 0) {
            cerr << "Performance (c/s)  : N/A" << endl;
        }
        else {
            cerr << "Performance (c/s)  : " << nb_cycles / d << endl;
        }
        // cerr << "Memory used        : " << usage.get_memory_size () << endl;
    }
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

