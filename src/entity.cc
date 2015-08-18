/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                   entity.cc                       |
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

#include <cstring>
#include <iomanip>
#include <list>
#include <map>
#include <vector>
#include <cassert>

#include "entity.h"
#include "sc_port.h"
#include "sc_signal.h"
#include "sc_module.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

equi_list_t equi_list;
equi_table_t equi_table = NULL;

ostream & operator << (ostream &, const entity &);
ostream & operator << (ostream &, const equi_t &);

struct predic4entity2equi_t_t {
    bool operator() (const entity & e1, const entity & e2) const {
        return e1.interface < e2.interface;
    }
};

typedef std::map <entity, equi_list_t::iterator, predic4entity2equi_t_t> entity2equi_it_t;


ostream & operator << (ostream & o, const entity2equi_it_t & m) {
    entity2equi_it_t::const_iterator i;
    for (i = m.begin(); i != m.end(); ++i) {
        o << (i->first) << " : " << *(i->second) << "\n";
    }
    return o;
}

entity2equi_it_t equi_map;


const equi_list_t & get_equi_list() {
    return equi_list;
}


unsigned int max(unsigned int a, unsigned int b) {
    return (a > b) ? a : b;
}


static int get_signal_table_size(const equi_list_t & e) {
    // bug fix : remove the 0x0 equi from the list
    equi_list_t::const_iterator i;
    int capacity = 0;
    for (i = e.begin(); i != e.end(); ++i) {
        //cerr << i->begin()->object->name ();
        const entity & ent = *(i->begin());
        int data_size = ent.data_size_in_bytes();
        int number_of_part = ((data_size - 1) / sizeof(tab_t)) + 1;
        capacity += number_of_part;
    }
    return capacity;
}


int get_signal_table_size() {
    return get_signal_table_size(equi_list);
}


sc_port_base * get_out_port(const equi_t & eq) {
    equi_t::const_iterator i;
    for (i = eq.begin(); i != eq.end(); ++i) {
        const char * type = i->kind();
        if ((type == sc_core::sc_out_string) || (type == sc_core::sc_inout_string)) {
            return i->port;
        }
    }
    return NULL;
}

static sc_port_base *get_localvar(equi_t & eq) {
    equi_t::iterator i;
    for (i = eq.begin(); i != eq.end(); ++i) {
        const char *type = i->kind();
        if (strcmp(type, "sc_localvar") == 0) {
            return i->port;
        }
    }
    return NULL;
}


static sc_port_base *get_signal(equi_t & eq) {
    equi_t::iterator i;
    for (i = eq.begin(); i != eq.end(); ++i) {
        const char * type = i->kind();
        if (strcmp(type, "sc_signal") == 0) {
            return i->port;
        }
    }
    return NULL;
}


equi_t & get_equi(const sc_interface & i) {
    return get_equi(i.get_pointer());
}


struct predic4tab_t2equi_t_t {
    bool operator() (const tab_t * t1, const tab_t * t2) const {
        return t1 < t2;
    }
};


#define get_equi_SPEEDUP
equi_t & get_equi(const tab_t * pointer) {
    // result variable
    equi_list_t::iterator i;
#if defined(get_equi_SPEEDUP)
    typedef std::map <const tab_t *, equi_list_t::iterator, predic4tab_t2equi_t_t> tab_t2equi_it_t;
    static tab_t2equi_it_t tab2equi_map;
    assert(pointer != NULL);

    // boost
    tab_t2equi_it_t:: /*const_ */ iterator it = tab2equi_map.find(pointer);
    if (it != tab2equi_map.end()) {
        i = it->second;
        return *i;
    }
#endif

    
    for (i = equi_list.begin(); i != equi_list.end(); ++i) {
        const equi_t::iterator & j = i->begin();
        const entity & e = *j;
        const sc_interface * f = e.interface;
        const tab_t * p = f->get_pointer();
        if (p == pointer) {
#if defined(get_equi_SPEEDUP)
            tab2equi_map[pointer] = i;
#endif
            return *i;
        }
    }
    cerr << "Internal error : get_equi(" << pointer << ")\n";
    exit(11);
}
#undef get_equi_SPEEDUP


bool has_equi( /*const */ sc_port_base & port) {
    entity e(port);
    entity2equi_it_t::/*const_ */iterator it = equi_map.find(e);
    if (it == equi_map.end()) {
        return false;
    }
    return true;
}


static bool is_register(const equi_t & eq) {
    return eq.size() == 1;
}

static int count_level(const char * s, char c) {
    int i;
    int counter = 0;
    for (i = 0; s[i] != '\0'; ++i) {
        if (s[i] == c) {
            ++counter;
        }
    }
    return counter;
}


const char * get_name(const equi_t & e) {
    equi_t::const_iterator top_iter = e.begin();
#ifdef CONFIG_DEBUG
    if (top_iter == e.end()) {
        cerr << "Internal error : no signal in " << e << endl;
        exit(25);
    }
    if (top_iter->object == NULL) {
        cerr << "Internal error : no object bound to ";
        cerr << *top_iter;
        cerr << endl;
        exit(26);
    }
    if (top_iter->object->name() == NULL) {
        cerr << "Internal error : signal has no name.\n";
        cerr << *top_iter;
        cerr << endl;
    }
#endif
    int top_level = count_level(top_iter->object->name(), '.');
    equi_t::const_iterator i;
    for (i = ++(e.begin()); i != e.end(); ++i) {
        int current_level = count_level(i->object->name(), '.');
        if (current_level < top_level) {
            top_iter = i;
            top_level = current_level;
        }
    }
#if 0
    cerr << "get_name of " << e;
    cerr << " return " << top_iter->object->name() << endl;
#endif
    return top_iter->object->name();
}


const char *get_name(const tab_t * pointer) {
    return get_name(get_equi(pointer));
}


static const sc_module & get_module(const string & s) {
    instances_set_t::iterator i;
    for (i = instances_set.begin(); i != instances_set.end(); ++i) {
        if (strcmp((*i)->name(), s.c_str()) == 0) {
            return **i;
        }
    }
    cerr << "Internal error : get_module\n";
    exit(27);
}


template < typename T > static equi_list_t::iterator get_equi(T & e) {
    entity2equi_it_t::iterator it = equi_map.find(entity(e));
    if (it == equi_map.end()) {
        return equi_list.end();
    }
    return it->second;
}


template < typename T > static void add_equi(equi_list_t::iterator & x, T & y) {
    entity e(y);
    x->push_back(e);
    equi_map[e] = x;
}


template < typename T > static
equi_list_t::iterator new_equi(T & x) { /* parameter is not 'const' because we need to modify pointer port/signal at end of elaboration step */
    equi_list_t::iterator it = equi_list.insert(equi_list.begin(), equi_t(1, entity(x)));
    equi_map[entity(x)] = it;
    return it;
}


// sort by number of connected ports  bound to the signal
static int operator <(const equi_t & e1, const equi_t & e2) {
    return (e1.size() < e2.size());
}

// sort by data size
/*
    int operator<(const equi_t &e1, const equi_t &e2) 
    { 
    return (e1.begin()->data_size () < e2.begin()->data_size ());
    }
    */
// random sort
/*
    int operator<(const equi_t &e1, const equi_t &e2) 
    { 
    typedef std::map<const equi_t*,int> m_t;
    static m_t m;
    if (m[&e1] == 0)
    m[&e1] = rand ();
    if (m[&e2] == 0)
    m[&e2] = rand ();
    return m[&e1] < m[&e2]; 
    }
    */

void sort_equi_list() {
    //load ();
    equi_list.sort();
    //equi_list.reverse ();
}


#if defined(DUMP_SIGNAL_STATS)
static unsigned int equi_real_size;
#endif

unsigned int get_sizeof_signals_table() {
    equi_list_t::iterator i;
    int table_size = 0;
    //cerr << "external table includes :\n";
    for (i = equi_list.begin(); i != equi_list.end(); ++i) {
        if (get_out_port(*i)) {
            continue;
        }
        if (get_signal(*i)) {
            continue;
        }
        //cerr << get_name (*i) << " ";
        const entity & ent = *(i->begin());
        int data_size = ent.data_size_in_bytes();
        int number_of_part = ((data_size - 1) / sizeof(tab_t)) + 1;
        table_size += number_of_part;
    }
    return table_size;
}


void create_signals_table() {
    if (dump_stage) {
        cerr << "Allocating signals table.\n";
    }
    unsigned int table_size = get_sizeof_signals_table();
    equi_table = new tab_t[table_size]; //(0xCD);
#if defined(DUMP_SIGNAL_STATS)
    equi_real_size = table_size;
#endif
#if defined(INIT_SIGNALS_TO_ZERO)
    memset(equi_table, 0, sizeof(tab_t) * table_size);
#else
    memset(equi_table, 0xCD, sizeof(tab_t) * table_size);
#endif
}


static void bind_equi_to_table(equi_t & e, tab_t * const pointer) {
    assert(pointer != NULL);
    equi_t::iterator i;
    for (i = e.begin(); i != e.end(); ++i) {
        sc_interface *const inter = i->interface;
        inter->set_pointer(pointer);
        // We can't initialize data to zero there 
        // because we can't write the right amount of bits.
        // It may produce some segmentation fault errors.
        // The pointer is aligned to 32 bits.
    }
}


void bind_to_table() {
    if (dump_stage) {
        cerr << "Binding ports/signals to the table.\n";
    }
    equi_list_t::iterator i;
    int index = 0;
    for (i = equi_list.begin(); i != equi_list.end(); ++i) {
        sc_interface *out = get_out_port(*i);
        if (out) {
            bind_equi_to_table(*i, out->get_pointer());
        }
        else {
            sc_interface * reg = get_signal(*i);
            if (reg == NULL) {
                reg = get_localvar(*i);
            }
            if (reg) {
                bind_equi_to_table(*i, reg->get_pointer());
            }
            else {
                bind_equi_to_table(*i, &(equi_table[index]));
                index += (i->begin()->data_size_in_bytes() - 1) / sizeof(tab_t) + 1;
            }
        }
    }
}


ostream & operator <<(ostream & o, const entity & e) {
#if 0
    o << "entity (";
    if (e.object)
        o << e.object->name();
    else
        o << "no object";
    if (e.interface)
        o << ", pointer = 0x" << hex << e.interface->get_pointer();
    else
        o << ", no interface";
    o << ")";
#else
    assert(e.object != NULL);
    o << e.object->name();
#endif
    return o;
}

ostream & operator <<(ostream & o, const equi_t & e) {
    bool nb = 0;
    equi_t::const_iterator i;
    for (i = e.begin(); i != e.end(); ++i) {
        const entity & ity = *i;
        o << ((nb++) ? " = " : "") << ity;
    }
    return o;
}


void print_table(ostream & o) {
    o << "Signal list\n" << "-----------\n";
    equi_list_t::const_iterator i;
    for (i = equi_list.begin(); i != equi_list.end(); ++i) {
        const equi_t & eq = *i;
#if 0
        o << i->begin()->interface->get_pointer() << " : ";
        o << (*i);
        o << "\n";
#else
        o << "(" << get_name(eq) << ") <=> " << eq << ".\n";
#endif
    }
}

void print_table_stats(ostream & o) {
#if defined(DUMP_SIGNAL_STATS)
    int nb_reg = 0;
    int nb_sig = 0;
    int real_size = 0;

    // count
    equi_list_t::const_iterator i;
    int num;
    for (num = 0, i = equi_list.begin(); i != equi_list.end(); ++num, ++i) {
        if (is_register(*i)) {
            ++nb_reg;
        }
        else {
            ++nb_sig;
        }
    }

    // print results
    o << "Statistics :\n";
    o << "#registers : " << nb_reg << endl;
    o << "#signals : " << nb_sig << endl;
    o << "current table size : " << equi_real_size << endl;
#endif
}


#ifdef DUMP_SIGNAL_STATS
typedef map < tab_t *, long long int > counter_t;
static counter_t counter;
long long int unnecessary = 0;
long long int total_assig = 0;
#endif

const char * get_module_name(const equi_t & eq) {
    if (!is_register(eq)) {
        /*
        cerr << "get_module_name doesn't work yet upon non-register.\n";
        */
        return "top_level";
    }
    const entity & ent = *(eq.begin());

#ifdef CONFIG_DEBUG
    if (ent.type != sc_core::entity::SIGNAL) {
        exit(28);
    }
#endif

    // get module name from sc_signal used like a register
    const char * sig_name = ent.object->name();
    const char * sep = strchr(sig_name, '.');

#ifdef CONFIG_DEBUG
    if (sep == NULL) {
        exit(30);
    }
#endif

    int end_pos = sep - sig_name;
    string module_name(sig_name, end_pos);
    // can't return the char* pointer from string since it will be invalid.
    return get_module(module_name).name();
}


static void merge_equi(equi_list_t::iterator & x, equi_list_t::iterator & y) {
    equi_t::iterator i;
    equi_t & y2 = *y;
    for (i = y2.begin(); i != y2.end(); ++i) {
        entity & e = *i;
        equi_map[e] = x;
    }

    x->merge(*y);
    equi_list.erase(y);
}


// sort by data size
#ifdef DATASIZE_SORT
int operator< (const equi_t & e1, const equi_t & e2) {
    return (e1.begin()->data_size() < e2.begin()->data_size());
}
#endif


// random sort
#ifdef RANDOM_SORT
int operator< (const equi_t & e1, const equi_t & e2) {
    typedef std::map < const equi_t *, int > m_t;
    static m_t m;
    if (m[&e1] == 0) {
        m[&e1] = rand();
    }
    if (m[&e2] == 0) {
        m[&e2] = rand();
    }
    return m[&e1] < m[&e2];
}
#endif


#if defined(DUMP_SIGNAL_STATS)
static unsigned int equi_real_size;
#endif


void bind(sc_signal_base & x) {
#if 0
    //defined(CONFIG_DEBUG)
    equi_list_t::iterator x_equi = get_equi(x);
    if ((x_equi != equi_list.end())) {
        cerr << "Internal error : Signal already in the table\n";
        return;
    }
#endif
    new_equi(x);
}


void bind(sc_port_base & x) {
    new_equi(x);
}


template < typename T > static void test_before_bind(sc_port_base & p1, T & p2) {
    bool t1 = (p1.kind() != NULL);
    bool t2 = (p2.kind() != NULL);
    if (t1 && t2) {
        return;
    }
    if (t1 == true) {
        cerr << "Trying to bind '" << p1.name() << "' to an unknown type.\n";
    }
    else if (t2 == true) {
        cerr << "Trying to bind '" << p2.name() << "' to an unknown type.\n";
    }
    else {
        cerr << "Binding failed. Please check the netlist description.\n";
    }
    exit(20040525);
}


template < typename T > static void tbind(sc_port_base & x, T & y) {
    // assert(x.get_pointer () != NULL); // x pointer may be NULL
    // assert(y.get_pointer () != NULL); // y pointer may be NULL
    equi_list_t::iterator x_equi = get_equi(x);
    equi_list_t::iterator y_equi = get_equi(y);
    if ((x_equi != equi_list.end()) && (y_equi != equi_list.end())) {
        // 2 equipotentials are equals
        merge_equi(x_equi, y_equi);
    }
    else if ((x_equi != equi_list.end())) {
        // add y
        add_equi(x_equi, y);
    }
    else if ((y_equi != equi_list.end())) {
        // add y
        add_equi(y_equi, x);
    }
    else {
        // add a new equi
        x_equi = new_equi(x);
        add_equi(x_equi, y);
    }
}


void bind(sc_port_base & p1, sc_port_base & p2) {
    test_before_bind(p1, p2);
    tbind(p1, p2);
}


void bind(sc_port_base & p1, sc_signal_base & s1) {
    test_before_bind(p1, s1);
    tbind(p1, s1);
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

