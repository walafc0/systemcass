/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_module_ext.h                 |
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

#ifndef __SC_MODULE_EXT_H__
#define __SC_MODULE_EXT_H__

#include "sc_fwd.h"
#include "internal_ext.h"
#include "sc_object.h"
#include "sc_sensitive.h"
#include "serialization_ext.h"

namespace sc_core {

class method_process_t;
class sc_module;
class sc_module_name;
class sc_port_base;
class sc_signal_base;

typedef void (sc_module::*SC_ENTRY_FUNC)();

extern method_process_t * create_method_process(const char *, SC_ENTRY_FUNC, sc_module &);


// ----------------------------------------------------------------------------
//  CLASS : sc_module
//
//  Module class.
// ----------------------------------------------------------------------------

typedef sc_signal_base sc_bind_proxy;
extern sc_bind_proxy SC_BIND_PROXY_NIL;


struct sc_module : public sc_object {

  //////////////////////
  // SYSTEMCASS SPECIFIC
private :
    void init();
  //////////////////////


  // LRM (?)
protected:
  sc_module();
  sc_module(const char * nm);
  sc_module(const sc_module_name & nm);

public:
  ~sc_module() {};

  /////////
  //// LRM
protected:
  void dont_initialize();

public:
  void operator () (
          /* const */ sc_bind_proxy& p001,
          /* const */ sc_bind_proxy& p002 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p003 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p004 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p005 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p006 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p007 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p008 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p009 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p010 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p011 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p012 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p013 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p014 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p015 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p016 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p017 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p018 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p019 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p020 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p021 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p022 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p023 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p024 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p025 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p026 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p027 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p028 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p029 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p030 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p031 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p032 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p033 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p034 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p035 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p036 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p037 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p038 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p039 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p040 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p041 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p042 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p043 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p044 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p045 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p046 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p047 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p048 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p049 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p050 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p051 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p052 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p053 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p054 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p055 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p056 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p057 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p058 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p059 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p060 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p061 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p062 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p063 = SC_BIND_PROXY_NIL,
          /* const */ sc_bind_proxy& p064 = SC_BIND_PROXY_NIL);

  /////////
  // These are protected so that user derived classes can refer to them.
  sc_sensitive sensitive;
  sc_sensitive_pos sensitive_pos;
  sc_sensitive_neg sensitive_neg;  
};


#define SC_MODULE(user_module_name) \
    struct user_module_name : public sc_core::sc_module

// the SC_HAS_PROCESS macro call must be followed by a ;
#define SC_HAS_PROCESS(user_module_name) \
    typedef user_module_name SC_CURRENT_USER_MODULE

#define SC_CTOR(user_module_name)     \
    SC_HAS_PROCESS(user_module_name); \
    user_module_name(sc_core::sc_module_name)

                              /* Converting `void (module_tag::*)()' to `void (sc_module::*)()' is OK as
                                 long as the dynamic type is correct.  C++ Standard 5.4 "Explicit type
                                 conversion", clause 7: a pointer to member of derived class type may be
                                 explicitly converted to a pointer to member of an unambiguous non-virtual
                                 base class type. */
#define SC_MAKE_FUNC_PTR(module_tag, func) \
    (/*static_cast<*/(sc_core::SC_ENTRY_FUNC)/*>*/ (&module_tag::func))

extern void declare_method_process(const char*,SC_ENTRY_FUNC,sc_module&);

#define DECLARE_METHOD_PROCESS(handle, name, module_tag, func) \
    declare_method_process(name,                               \
    SC_MAKE_FUNC_PTR( module_tag, func ),                      \
    *this)


#define SC_METHOD(func)                                                          \
{                                                                                \
    DECLARE_METHOD_PROCESS(func ## _handle, #func, SC_CURRENT_USER_MODULE,func); \
}

#define SC_MAKE_SAVE_PTR(module_tag, func) \
    (/*static_cast<*/(sc_core::save_fct_t1)/*>*/ (&module_tag::func))

extern void declare_save_handler(const char*,save_fct_t1,sc_module&);

#define DECLARE_SAVE_HANDLER(handle, name, module_tag, func) \
    declare_save_handler (name,                              \
    SC_MAKE_SAVE_PTR( module_tag, func ),                    \
    *this)


#define SAVE_HANDLER(func)                                                     \
{                                                                              \
    DECLARE_SAVE_HANDLER(func ## _handle, #func, SC_CURRENT_USER_MODULE,func); \
}

} // end of namespace sc_core

#endif /* __SC_MODULE_EXT_H__ */

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

