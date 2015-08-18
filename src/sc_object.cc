/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_object.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
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


#include <cstdio>
#include <cstring> //strdup
#include <string>
#include <map>
#include <cassert>

#include "sc_object.h"
#include "internal.h"
#include "sc_signal.h"
#include "module_hierarchy.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


using namespace std;

namespace sc_core {

static void gen_name (const char *prefix, string &s) {
    s = prefix;
    static int i = 0;
    char ic[10];
    sprintf (ic,"%d",i++);
    s += ic;
}


static void build_complete_name (const char *name, string &out) {
    out = "";
    module_name_stack_t::const_iterator i;
    for (i = module_name_stack.begin (); i != module_name_stack.end (); ++i) {
        const string &module_name = *i;
        out += (module_name + ".");
    }
    if (name) {
        out += name;
    }
    else {
        out[out.length ()-1] = '\0';
    }
}


typedef std::map<const sc_object * const, string> object2name_t;
static object2name_t object2basename;
static object2name_t object2fullname;

struct object_infos_t {
    const char * kind_string;
};

typedef std::map<const sc_object * const, object_infos_t> object2infos_t;
static object2infos_t object2infos;


// We initialize SC_BIND_PROXY_NIL there to make sure object2infos,
// object2fullname, and object2basename are already initialized.
// SC_BIND_PROXY_NIL should be declared into sc_module.cc but it prevents us
// to force the initialization order.
const char * SC_BIND_PROXY_NIL_string = "SC_BIND_PROXY_NIL";
sc_bind_proxy SC_BIND_PROXY_NIL(SC_BIND_PROXY_NIL_string, NULL);


// ----------------------------------------------------------------------------
//  FUNCTION : sc_gen_unique_name
//                                               
// ----------------------------------------------------------------------------
const char * sc_gen_unique_name(const char * basename_) {
    string s;
    gen_name(basename_,s);
    return strdup (s.c_str());
}


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//                                               
// ----------------------------------------------------------------------------

const char * const sc_object::kind_string = "sc_object";

void sc_object::set_kind(const char * k) {
    object2infos[this].kind_string = k;
}


void sc_object::init() {
    set_kind("sc_object");
    add_child(*this);
}


sc_object::sc_object() {
    string noname;
    gen_name("noname_", noname);
    object2basename[this] = noname;
    build_complete_name(noname.c_str(), object2fullname[this]);
    init();
}


sc_object::sc_object(const char * name_) {
    const char * temp;
    if (name_ == NULL) {
#ifdef CONFIG_DEBUG
        if (module_name_stack.empty()) {
            cerr << "Internal error : module_name_stack is empty.";
            exit(21092005);
        }
#endif
        string & module_name = module_name_stack.back();
        temp = module_name.c_str();
    }
    else {
        temp = name_;
    }
    object2basename[this] = temp;
    build_complete_name(name_, object2fullname[this]);
    init();
}


const char * sc_object::basename() const {
    return object2basename[this].c_str();
}


const char * sc_object::name() const {
    object2name_t::iterator i = object2fullname.find(this);
#ifdef CONFIG_DEBUG
    if (i == object2fullname.end()) {
        cerr << "Internal error : can't find name of " << this << "\n";
        exit (90);
    }
#endif
    return i->second.c_str();
}


void sc_object::rename(const char * newname) const {
    object2basename[this] = newname;
    build_complete_name (newname,object2fullname[this]);
}


const char * sc_object::kind() const {
    object2infos_t::iterator i = object2infos.find(this);
#ifdef CONFIG_DEBUG
    if (i == object2infos.end()) {
        cerr << "Internal error : can't find kind of " << this << "\n";
        exit(90);
    }
#endif
    return i->second.kind_string;
}


sc_object::~sc_object() {
    if (save_on_exit) {
        sc_save_simulation(save_on_exit);
        save_on_exit = NULL;
    }
    object2fullname.erase(this);
    object2basename.erase(this);
    object2infos.erase(this);
}


std::ostream & operator << (std::ostream & os, const sc_object & obj) {
    return os << obj.name ();
}


/* virtual */ 
const std::vector<sc_object *> & sc_object::get_child_objects() const {
    return sc_core::get_child_objects(*this);
}


sc_object * sc_object::get_parent_object() const {
    return sc_core::get_parent_object(*this);
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

