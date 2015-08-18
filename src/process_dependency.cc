/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                  process_dependancy.cc            |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   21_09_2004                      |
  |                                                             |
  \------------------------------------------------------------*/

/* 
 * This file is part of the Disydent Project
 * Copyright (C) Laboratoire LIP6 - D�partement ASIM
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

#include <set>
#include <iostream>
#include <fstream>

#include "assert.h"
#include "process_dependency.h"
#include "methodprocess_dependency.h"
#include "simplify_string.h"
#include "sc_fwd.h"
#include "sc_module.h"
#include "sc_ver.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

bool ProcessDependency::operator < (const ProcessDependency &a) const {
    if (a.source < source) {
        return false;
    }
    if (a.destination < destination) {
        return false;
    }
    return true;
}


static void dot_write (ofstream & o, const ProcessDependencyGraph & g) {
    string s;
    ProcessDependencyGraph::const_iterator it;
    for (it = g.begin (); it != g.end (); ++it) {
        string name;
        name = it->source->module->name();
        name += "_";
        name += it->source->name;
        o << simplify_name(name.c_str(),s);
        o << " -> ";
        name = it->destination->module->name();
        name += "_";
        name += it->destination->name;
        o << simplify_name(name.c_str(),s);
        o << "\n";
    }
}


static void dot_write(ofstream &o, const ProcessDependencyList &l) {
    const method_process_t * old = NULL;
    string s;
    ProcessDependencyList::const_iterator it;
    for (it = l.begin (); it != l.end (); ++it) {
        const method_process_t * current = *it;
        if (old) {
            string name;
            name = old->module->name();
            name += "_";
            name += old->name;
            o << simplify_name(name.c_str(),s);
            o << " -> ";
            name = current->module->name();
            name += "_";
            name += current->name;
            o << simplify_name(name.c_str(),s);
            o << "\n";
        }
        old = current;
    }
}


bool ProcessDependencyGraph2dot(const char * name, const ProcessDependencyGraph& g) {
    if (!name) {
        return false;
    }
    string filename;
    filename =  name;
    filename += ".dot";
    ofstream o;
    o.open (filename.c_str(),ios::out | ios::trunc);
    if (o.is_open () == false) {
        return false;
    }
    o << "// Ordered process list\n"
        "// Generated by "
        << sc_version () << "\n";
    o << "strict digraph " << name << " {\n";
    dot_write (o,g);
    o << "}\n";
    o.close ();
    if (dump_stage) {
        cerr << "Port Dependency Graph written into '" 
             << filename << "'.\n";
    }
    return true;
}


bool ProcessDependencyList2dot(const char * name, const ProcessDependencyList& l) {
    if (!name) {
        return false;
    }
    string filename;
    filename =  name;
    filename += ".dot";
    ofstream o;
    o.open (filename.c_str(),ios::out | ios::trunc);
    if (o.is_open () == false) {
        return false;
    }
    o << "// Ordered process list\n"
        "// Generated by "
        << sc_version () << "\n";
    o << "digraph " << name << " {\n";
    dot_write (o,l);
    o << "}\n";
    o.close ();
    return true;
}


static bool is_leef(const SignalDependencyGraph & g, const equi_t * e) {
    SignalDependencyGraph::const_iterator it;
    for (it = g.begin(); it != g.end(); ++it) {
        if (it->source == e) {
            return false;
        }
    }
    return true;
}


static bool has_all_leef(const SignalDependencyGraph & sig, const method_process_t * m) {
    SignalDependencyGraph::const_iterator sig_it;
    for (sig_it = sig.begin(); sig_it != sig.end(); ++sig_it) {
        if ((sig_it->method == m) && (!is_leef (sig, sig_it->destination))) {
            return false;
        }
    }
    return true;
}


static const method_process_t * get_best_process(const SignalDependencyGraph & sig) {
    SignalDependencyGraph::const_iterator sig_it;
    for (sig_it = sig.begin(); sig_it != sig.end(); ++sig_it) {
        if (has_all_leef(sig, sig_it->method)) {
            return sig_it->method;
        }
    }
    return NULL;
}


/*
 *   Remove signals
 */
static void remove_leefs (SignalDependencyGraph  & g, const method_process_t & m) {
    SignalDependencyGraph::iterator it;
    it = g.begin();
    while (it != g.end()) {
        const method_process_t * cur_m = it->method;
        if ((cur_m == &m) && (is_leef (g,it->destination))) {
            SignalDependencyGraph::iterator x = it++;
            g.erase (x);
        }
        else {
            ++it;
        }
    }
}


#if defined(UNUSED)
static void print_signals (ostream & o,
        const SignalDependencyGraph & sig,
        const method_process_t * method,
        bool source) {
    int count = 0;
    SignalDependencyGraph::const_iterator it;
    for (it = sig.begin(); it != sig.end(); ++it) {
        const SignalDependency & dep = *it;
        const method_process_t * cur_m = dep.method;
        if (cur_m == method) {
            const equi_t * e = (source) ? dep.source : dep.destination;
            o << ((count++)?",":"") << get_name (*e);
        }
    }
}
#endif


static void print_leef_list (SignalDependencyGraph & sig) {
    typedef map<const equi_t *, const method_process_t *> table_t;
    table_t table;

    // For each arrow, add destination node into table
    SignalDependencyGraph::const_iterator sig_it;
    for (sig_it = sig.begin(); sig_it != sig.end(); ++sig_it) {
        const SignalDependency sd = *sig_it;
        const equi_t * equi = sd.destination;
        const method_process_t * method = sd.method;
        table[equi] = method;
    }

    // For each arrow, remove source node into table
    for (sig_it = sig.begin(); sig_it != sig.end(); ++sig_it) {
        const SignalDependency sd = *sig_it;
        const equi_t * e = sd.source;
        table_t::const_iterator tab_it = table.find(e);
        if (tab_it != table.end()) {
            table.erase(e);
        }
    }

    typedef multimap<const method_process_t *, const equi_t *> table2_t;
    typedef pair<const method_process_t *, const equi_t *> table2_pair_t;
    table2_t table2;

    // Build table2
    table_t::const_iterator it;
    for (it = table.begin(); it != table.end(); ++it) {
        const method_process_t * method = it->second;
        const equi_t * equi = it->first;
        table2_pair_t pair = table2_pair_t(method,equi);
        table2.insert(pair);
    }

    // Print leef list
    cerr << "Please split following methods :";
    table2_t::const_iterator low_it = table2.begin();
    table2_t::const_iterator up_it;
    while (low_it != table2.end()) {
        const method_process_t * method = low_it->first;
        const char * method_name = method->name;
        const sc_module * module = method->module;
        const char * module_name = module->name();
        cerr << "\n\t - " << module_name << "::" << method_name;
        cerr << "() to evaluate '";
        int count = 0;
        up_it = table2.upper_bound(low_it->first);
        table2_t::const_iterator it2;
        for (it2 = low_it; it2 != up_it; ++it2) {
            const equi_t * equi = it2->second;
            const char * signal_name = get_name(*equi);
            cerr << ((count++) ? ",":"") << signal_name;
        }
        cerr << "' signal(s) separetly";
        low_it = up_it;
    }
    cerr << "\n";
}


static void help2resolve(SignalDependencyGraph & sig) {
    print_leef_list(sig);
    MethodProcessDependencyGraph2dot ("methodprocess_graph", sig);
    cerr << "Please look at 'methodprocess_graph.dot' "
        "to know how to get ride of circular dependencies.\n";
    if (dump_all_graph) {
        SignalDependencyGraph2dot ("reduced_signal_graph", sig);
    }
}


ProcessDependencyList * MakeProcessDependencyList(const SignalDependencyGraph & _sig_g) {
    if (dump_stage) {
        cerr << "Making process dependency list...\n";
    }
    ProcessDependencyList * mod_l = new ProcessDependencyList();
    SignalDependencyGraph sig_g = _sig_g;
    while (!sig_g.empty()) {
        const method_process_t * process = get_best_process(sig_g);
        if (process == NULL)
        {
            cerr << "Internal Error : Unable to select the best process to schedule.\n";
            help2resolve(sig_g);
            exit (31032005); // 197
        }
        mod_l->push_front(process);
        if (dump_stage) {
            cerr << "Process found : " << *process << "\n";
        }
        remove_leefs (sig_g, * process);
    }
    return mod_l;
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

