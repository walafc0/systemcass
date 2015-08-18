/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_port.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
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


#include <iomanip>
#include <map>
#include <cassert>

#include "sc_port.h"
#include "sc_signal.h"
#include "sc_module.h"
#include "entity.h"
#include "global_functions.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
    extern char unstable;
    char unstable = 0; // not in sc_core namespace because dynamic link support C linkage only
    int32_t pending_write_vector_nb = 0;
}

using namespace std;

namespace sc_core {

const char * const sc_port_base::kind_string = "sc_port";
const char * const sc_signal_base::kind_string = "sc_signal";

unsigned int pending_write_vector_capacity = 512;
pending_write_vector_t pending_write_vector = NULL;
extern equi_list_t equi_list;

const char * get_module_name (const tab_t * pointer) {
	const equi_t & eq = get_equi (pointer);
    return get_module_name (eq);
}

port2module_t port2module;

// KIND STRING
const char * const sc_inout_string = "sc_inout";
const char * const sc_in_string    = "sc_in";
const char * const sc_out_string   = "sc_out";


// ----------------------------------------------------------------------------
//  CLASS : sc_port_base
//
//                                               
// ----------------------------------------------------------------------------

sc_port_base::sc_port_base() {
	init ();
}


sc_port_base::sc_port_base(const char * name_) : sc_object(name_) {
    init ();
}


sc_port_base::sc_port_base(const sc_port_base & parent_) : sc_object (parent_.name ()) {
    init ();
}


void sc_port_base::init() {
#ifdef CONFIG_DEBUG
    if (modules_stack.empty()) {
        cerr << "Internal error : modules stack empty\n";
        exit (9);
    }
#endif
    const sc_module * last_module = sc_core::modules_stack.top();
    port2module[this] = last_module;
    set_kind(kind_string);
}


const sc_module & sc_port_base::get_module() const {
    port2module_t::iterator i = port2module.find ((sc_port_base *) this);
    if (i == port2module.end()) {
        cerr << "Internal error : Modules contain ports. "
            "SystemCASS needs to identify the module that contains the following port : '" 
            << name()
            << "'\n";
        exit (17);
    }
    const sc_module * m = i->second;
    return *m;
}


static bool check_multiwriting2port_error_message = true;

void sc_port_base::check_multiwriting2port() const {
    static std::map<sc_port_base *, double> s;
    double t = sc_simulation_time();
    if (t == 0) {
        return;
    }
    sc_port_base * port = (sc_port_base *) this;
    if ((s[port] == t) && (check_multiwriting2port_error_message)) {
        check_multiwriting2port_error_message = 0;
        if (use_port_dependency) {
            std::cerr << "Error at cycle #" << t << " : "
                "SystemCASS allows only 1 writing for each ports/signals.\n"
                << "Functions write several times into '" << name () << "'.\n";
        }
        else {
            std::cerr << "Error : "
                "Multiwriting to port assertion works only "
                "when SystemCASS uses port dependency information "
                "(--p parameter).\n";
        }
        sc_core::sc_stop();
        exit (31072006); // 6
    }
    else {
        s[port] = t;
    }
}


std::ostream & operator << (std::ostream & o, const sc_port_base & p) {
    return o << p.name();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_base
//
//  The sc_signal<T> primitive channel class.
// ----------------------------------------------------------------------------
void sc_signal_base::init() {
    set_kind(kind_string);
    bind(*this);
}


sc_signal_base::sc_signal_base() { 
    init();
}


sc_signal_base::sc_signal_base(const char * name_) : sc_object(name_) { 
    init ();
}


sc_signal_base::sc_signal_base(const char * name_, void *) : sc_object(name_) {
    // this overload is only used for SC_BIND_PROXY_NIL constant.
    // this signal should not be added to the signal list.
}


sc_signal_base::~sc_signal_base() {}


/*
 * Signals copy
 */

#ifdef DUMP_SIGNAL_STATS
typedef map<tab_t *, long long int> counter_t;
static counter_t counter;
long long int unnecessary = 0;
long long int total_assig = 0;
#endif

} // end of sc_core namespace


extern "C" {

    void update() {
#if defined(DUMP_STAGE)
            cerr << "Updating... ";
#endif
            // stl vectors are too slow
            // memcopy is not better
            // signal table sorting doesn't give any better performance	
#if defined(DUMP_STAGE)
            cerr << "(" << pending_write_vector_nb 
                << " internal pending writings) ";
#endif
            unsigned int i;
            for (i = 0; i < pending_write_vector_nb; ++i) {
#define iter (sc_core::pending_write_vector[i])
#ifdef CONFIG_DEBUG
                if (iter.pointer == NULL) {
                    cerr << "Internal error : trying to apply a posted write from an unassigned signal/port\n";
                    exit (8);
                }
#endif	
#ifdef DUMP_SIGNAL_STATS
                if (*(iter.pointer) == iter.value) {
                    unnecessary++;
                }
                counter[iter.pointer]++;
#endif
                *(iter.pointer) = iter.value;	
#undef iter
            }
#ifdef DUMP_SIGNAL_STATS
            total_assig += pending_write_vector_nb;
#endif
            pending_write_vector_nb = 0;

#if defined(DUMP_STAGE)
            cerr << "done.\n";
#endif
        }

} // end of extern "C"

namespace sc_core {

void print_registers_writing_stats (ostream & o) {
#ifdef DUMP_SIGNAL_STATS
    o << "signal index / name / usage (>1%)\n";
    o << setprecision(2);
    double t = sc_simulation_time();
    if (t == 0) {
        o << "Warning : simulation too short.\n";
        t = 0.01;
    }
    if (total_assig == 0) {
        return;
    }
    counter_t::iterator k;
    for (k = counter.begin(); k != counter.end(); ++k) {
        double usage = k->second / t * 100;
        if (usage <= 1) {
            continue;
        }
        o << k->first << "   " << get_name (k->first) << "   " << usage << "%\n";
    }
    typedef map<string, int> counter_module_t;
    counter_module_t counter_by_module;
    for (k = counter.begin(); k != counter.end(); ++k) {
        string module_name = get_module_name (k->first);
        counter_by_module[module_name] += k->second;	
    }
    o << "module name / usage\n";
    counter_module_t::iterator i;
    for (i = counter_by_module.begin(); i != counter_by_module.end(); ++i) {
        o << i->first << "   " << (i->second * 100 / total_assig) << "%\n";
    }
    cerr << "average of assignment number per cycle " << (total_assig / t) << "\n";
    cerr << (unnecessary * 100 / total_assig) << "% of assignment are unecessary\n";
#else
    cerr << "Register usage not available.\n";
#endif
}

static bool is_bound(/*const*/ sc_port_base & port) {
    const tab_t * pointer = port.get_pointer();
    //assert(pointer != NULL);
    if (pointer == NULL) {
        return false; // case : sc_in not bound
    }
    return has_equi (port);
}

static void check_port(/*const*/ sc_port_base & port) {
    if (!is_bound (port)) {
        cerr << "Error : '" << port.name() << "' port"
            " (" << port.kind() << ")"
            " is not bound.\n";
        exit (29);
    }
}


void check_all_ports() {
    if (dump_stage) {
        cerr << "checking ports...";
    }
    port2module_t::/*const_*/iterator i;
    for (i = port2module.begin(); i != port2module.end(); ++i) {
        /*const*/ sc_port_base *port = i->first;
        assert(port != NULL);
        check_port(*port);
    }
    if (dump_stage) {
        cerr << " done.";
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

