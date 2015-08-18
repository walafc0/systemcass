/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_module.cc                    |
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

#include <stdarg.h>
#include <vector>
#include <set>

#include "sc_module.h"
#include "sc_module_name.h"
#include "sc_sensitive.h"
#include "module_hierarchy.h"
#include "serialization.h" // set_save_handler
#include "sc_port.h"
#include "sc_signal.h"
#include "sc_clock.h" // is_clock
#include "entity.h"
#include <cassert>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//
using namespace std;

// ----------------------------------------------------------------------------
//
//                                               
// ----------------------------------------------------------------------------

namespace sc_core {

instances_set_t instances_set;   
instances_list_t temp_list;   
method_process_list_t method_process_list;
module_name_stack_t module_name_stack;
modules_stack_t modules_stack;
method_process_t * method;

void declare_method_process(const char * name, SC_ENTRY_FUNC func, sc_module & module) {
  method = create_method_process(name, func, module);
  method_process_list.push_back(method); 
}


ostream & operator << (ostream &o, instances_set_t &l) {
  instances_set_t::iterator i;
  for (i = l.begin(); i != l.end(); ++i) {
    o << (*i)->name() << " ";
  }
  return o << endl;
}


template <typename T>
static std::ostream& operator << (std::ostream & o, const list<T> &l) {
  typename list<T>::const_iterator i;
  for (i = l.begin(); i != l.end(); ++i) {
    o << (*i) << " ";
  }
  return o << endl;
}


template <typename T>
static std::ostream& operator << (std::ostream &o, const list<T *> &l) {
  typename list<T *>::const_iterator i;
  for (i = l.begin(); i != l.end(); ++i) {
    o << (**i) << " ";
  }
  return o << endl;
}


bool is_clock (const sc_interface & inter) {
    equi_t & e = get_equi(inter);
    equi_t::iterator i;
    for (i = e.begin(); i != e.end(); ++i) {
        if (i->kind() == sc_clock::kind_string) {
            return true;
        }
    }
    return false;
}


// ----------------------------------------------------------------------------
//  CLASS : method_process_t
//                                               
// ----------------------------------------------------------------------------
method_process_t::method_process_t(const char * nm, SC_ENTRY_FUNC fn, sc_module & mod) {
  name = nm;
  func = fn; 
  module = &mod;
  dont_initialize = false;
}


static bool is_register (const sc_interface & inter) {
    equi_t & e = get_equi(inter);
    if (e.size() != 1) {
        return false;
    }
    
    sc_object & obj = *(e.begin()->object);
    if (obj.kind() != sc_signal_base::kind_string) {
        return false;
    }
    return true;
}


bool method_process_t::is_combinational(void) {
    if (sensitivity_list.empty()) {
        return false;
    }
    sensitivity_list_t::iterator i;
    for (i = sensitivity_list.begin(); i != sensitivity_list.end(); ++i) {
#if defined(CONFIG_DEBUG) && 0
        if (i->get_interface() == NULL) {
            cerr << "'" << i << "' is unbound.\n";
            exit(121);
        }
#endif
        if ((i->get_flag() == sc_event::VAL) && (!is_register(i->get_interface()))) {
            return true;
        }
    }
    return false;
}


bool method_process_t::is_transition(void) {
    if (sensitivity_list.empty()) {
        return false;
    }
    sensitivity_list_t::iterator i;
    for (i = sensitivity_list.begin(); i != sensitivity_list.end(); ++i) {
        if (i->get_flag() != sc_event::POS) {
            return false;
        }
        // check if the port is a clock
        if (is_clock(i->get_interface()) == false) {
            return false;
        }
    }
    return true;
}


bool method_process_t::is_genmoore(void) {
    // sensitivity list of moore functions includes :
    // - register (signal unconnected)
    // - negative edge of port which is bound to a clock signal
    if (sensitivity_list.empty()) {
        return false;
    }
    sensitivity_list_t::iterator i;
    for (i = sensitivity_list.begin(); i != sensitivity_list.end(); ++i) {
        if ((i->get_flag() == sc_event::VAL) && (is_register(i->get_interface()))) {
            continue;
        }
        if ((i->get_flag() == sc_event::NEG) && (is_clock(i->get_interface()) == true)) {
            continue;
        }
        return false;
    }
    return true;
}


std::ostream & operator << (std::ostream & o, const method_process_t & m) {
    return o << *(m.module) 
        << "->" 
        << m.name 
        << "() when " 
        << m.sensitivity_list;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_module
//                                               
// ----------------------------------------------------------------------------


sc_module::sc_module(void) :
    sc_object(NULL),
    sensitive(this) {
  init();
}

sc_module::sc_module(const char * nm) :
    sc_object(NULL),
    sensitive(this) {
  assert(nm != NULL);
  init();
}


sc_module::sc_module(const sc_module_name & nm) :
    sc_object(NULL),
    sensitive(this) {
  init();
}


void sc_module::init() {
    instances_set.insert(this);
    temp_list.push_back(this);
    modules_stack.top() = this;
    set_save_handler(*this, NULL);
}

void sc_module::dont_initialize() {
    sc_core::valid_method_process();
}


void declare_save_handler(const char * name, save_fct_t1 func, sc_module & module) {
    sc_core::set_save_handler(module, func);
}

typedef std::list<sc_port_base*> port_list_t;

// Build a port list owned by the module mod
static void get_port_list(const sc_module & mod, port_list_t & pl) {
    port2module_t::iterator i;
    for (i = port2module.begin(); i != port2module.end(); ++i) {
        if (i->second == &mod) {
            pl.push_back((sc_port_base *) (i->first));
        }
    }
}


#define BIND(sig) bind(*this, p, port_list, sig)

static void bind(sc_module & mod, 
        port_list_t::iterator & port_it, 
        port_list_t & port_list, 
        sc_bind_proxy & sig) {
    if (&sig == &SC_BIND_PROXY_NIL) { 
        return;
    }
    if (port_it == port_list.end()) {
        cerr << "error : binding port in ";
        cerr << mod.name();
        cerr << " module.\n";
        exit(7);
    }
    bind(**(port_it++), sig);
}


void sc_module::operator () ( /* const */ sc_bind_proxy & p001,
        /* const */ sc_bind_proxy & p002,
        /* const */ sc_bind_proxy & p003,
        /* const */ sc_bind_proxy & p004,
        /* const */ sc_bind_proxy & p005,
        /* const */ sc_bind_proxy & p006,
        /* const */ sc_bind_proxy & p007,
        /* const */ sc_bind_proxy & p008,
        /* const */ sc_bind_proxy & p009,
        /* const */ sc_bind_proxy & p010,
        /* const */ sc_bind_proxy & p011,
        /* const */ sc_bind_proxy & p012,
        /* const */ sc_bind_proxy & p013,
        /* const */ sc_bind_proxy & p014,
        /* const */ sc_bind_proxy & p015,
        /* const */ sc_bind_proxy & p016,
        /* const */ sc_bind_proxy & p017,
        /* const */ sc_bind_proxy & p018,
        /* const */ sc_bind_proxy & p019,
        /* const */ sc_bind_proxy & p020,
        /* const */ sc_bind_proxy & p021,
        /* const */ sc_bind_proxy & p022,
        /* const */ sc_bind_proxy & p023,
        /* const */ sc_bind_proxy & p024,
        /* const */ sc_bind_proxy & p025,
        /* const */ sc_bind_proxy & p026,
        /* const */ sc_bind_proxy & p027,
        /* const */ sc_bind_proxy & p028,
        /* const */ sc_bind_proxy & p029,
        /* const */ sc_bind_proxy & p030,
        /* const */ sc_bind_proxy & p031,
        /* const */ sc_bind_proxy & p032,
        /* const */ sc_bind_proxy & p033,
        /* const */ sc_bind_proxy & p034,
        /* const */ sc_bind_proxy & p035,
        /* const */ sc_bind_proxy & p036,
        /* const */ sc_bind_proxy & p037,
        /* const */ sc_bind_proxy & p038,
        /* const */ sc_bind_proxy & p039,
        /* const */ sc_bind_proxy & p040,
        /* const */ sc_bind_proxy & p041,
        /* const */ sc_bind_proxy & p042,
        /* const */ sc_bind_proxy & p043,
        /* const */ sc_bind_proxy & p044,
        /* const */ sc_bind_proxy & p045,
        /* const */ sc_bind_proxy & p046,
        /* const */ sc_bind_proxy & p047,
        /* const */ sc_bind_proxy & p048,
        /* const */ sc_bind_proxy & p049,
        /* const */ sc_bind_proxy & p050,
        /* const */ sc_bind_proxy & p051,
        /* const */ sc_bind_proxy & p052,
        /* const */ sc_bind_proxy & p053,
        /* const */ sc_bind_proxy & p054,
        /* const */ sc_bind_proxy & p055,
        /* const */ sc_bind_proxy & p056,
        /* const */ sc_bind_proxy & p057,
        /* const */ sc_bind_proxy & p058,
        /* const */ sc_bind_proxy & p059,
        /* const */ sc_bind_proxy & p060,
        /* const */ sc_bind_proxy & p061,
        /* const */ sc_bind_proxy & p062,
        /* const */ sc_bind_proxy & p063,
        /* const */ sc_bind_proxy & p064)
{
    port_list_t port_list;
    get_port_list(*this, port_list);
    cerr << "port list : " << port_list << endl;
    port_list_t::iterator p = port_list.begin();
    BIND(p001);
    BIND(p002);
    BIND(p003);
    BIND(p004);
    BIND(p005);
    BIND(p006);
    BIND(p007);
    BIND(p008);
    BIND(p009);
    BIND(p010);
    BIND(p011);
    BIND(p012);
    BIND(p013);
    BIND(p014);
    BIND(p015);
    BIND(p016);
    BIND(p017);
    BIND(p018);
    BIND(p019);
    BIND(p020);
    BIND(p021);
    BIND(p022);
    BIND(p023);
    BIND(p024);
    BIND(p025);
    BIND(p026);
    BIND(p027);
    BIND(p028);
    BIND(p029);
    BIND(p030);
    BIND(p031);
    BIND(p032);
    BIND(p033);
    BIND(p034);
    BIND(p035);
    BIND(p036);
    BIND(p037);
    BIND(p038);
    BIND(p039);
    BIND(p040);
    BIND(p041);
    BIND(p042);
    BIND(p043);
    BIND(p044);
    BIND(p045);
    BIND(p046);
    BIND(p047);
    BIND(p048);
    BIND(p049);
    BIND(p050);
    BIND(p051);
    BIND(p052);
    BIND(p053);
    BIND(p054);
    BIND(p055);
    BIND(p056);
    BIND(p057);
    BIND(p058);
    BIND(p059);
    BIND(p060);
    BIND(p061);
    BIND(p062);
    BIND(p063);
    BIND(p064);
}


ostream & operator << (ostream & o, const sc_module & m) {
    return o << m.name();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_module_name
//                                               
// ----------------------------------------------------------------------------

sc_module_name::sc_module_name(const char * name_) : m_pushed(true) {
    m_name = name_;
    init();
}


sc_module_name::sc_module_name(const sc_module_name & module) : m_pushed(false) {
    m_name = module.m_name;
}


void sc_module_name::init() {
    sc_core::module_name_stack.push_back(m_name);
    modules_stack.push(NULL);
}


sc_module_name::~sc_module_name() {
    if (m_pushed == false) {
        return;
    }
    assert(sc_core::module_name_stack.empty() == false);
    sc_core::module_name_stack.pop_back();
    modules_stack.pop();
    assert(temp_list.empty() == false);
    sc_module * last1 = temp_list.back();
    temp_list.pop_back();
    sc_module * last2 = (temp_list.empty()) ? NULL : temp_list.back();
    set_parent(*last1, last2);
}


std::ostream & operator << (std::ostream & o, const sc_core::sc_module_name & n) {
    return o << (const char *) n;
}

/////////////////////////////////////////:

static void check_method_process(const method_process_t & m) {
    if (m.dont_initialize == false) {
        assert(m.module != NULL);
#ifdef CONFIG_DEBUG
        std::cerr << "Warning : SystemCASS doesn't perform SC_METHOD(S) initializations.\n"
            << "Please turn off automatic initialization for '" << m.name 
            << "' method of '" << m.module->name() << "' module"
            " by calling 'dont_initialize()' function.\n"
            "Example :\n"
            "  SC_METHOD(transition);\n"
            "  sensitive << clk;\n"
            "  dont_initialize();\n";
#endif
    }
}


void check_all_method_process() {
    method_process_list_t::const_iterator i;
    for (i = sc_core::method_process_list.begin(); i != sc_core::method_process_list.end(); ++i) {
        check_method_process(**i);
    }
}


void valid_method_process() {
    method_process_t * m = sc_core::method_process_list.back();
    m->dont_initialize = true;
}


method_process_t * create_method_process(const char * name, SC_ENTRY_FUNC func, sc_module & module) {
    return new method_process_t(name, func, module);
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

