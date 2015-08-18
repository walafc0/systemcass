/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_trace.h                      |
|                                                             |
| Author  :                 Kingbo Paul-Jerome                |
|                           Buchmann Richard                  |
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

#ifndef __SC_TRACE_EXT_H__
#define __SC_TRACE_EXT_H__

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "sc_fwd.h"
#include "alias.h"
#include "sc_module_ext.h"
#include "sc_localvar.h"



namespace sc_core {

using namespace sc_dt;



struct signal2trace {
    const sc_interface * inter;
    const char * alias;
    unsigned int bit_size;
    tab_t * pointer;
    std::string nomSig;
};



template< typename T > struct bits_number_getter {};


#define BITS_NUMBER_GETTER_DEF(newt, typ, expr)           \
    template<newt> struct bits_number_getter<typ> {       \
        typedef typ val_t;                                \
        static inline unsigned int get() { return expr; } \
    }


BITS_NUMBER_GETTER_DEF(, bool          , 1);
BITS_NUMBER_GETTER_DEF(, char          , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, signed   char , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, unsigned char , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, unsigned short, 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, signed   short, 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, signed   int  , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, unsigned int  , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, signed   long , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, unsigned long , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, float         , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, sc_unsigned   , 8 * sizeof(unsigned int));
BITS_NUMBER_GETTER_DEF(, sc_signed     , 8 * sizeof(int));
BITS_NUMBER_GETTER_DEF(, double        , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, uint64        , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(, int64         , 8 * sizeof(val_t));
BITS_NUMBER_GETTER_DEF(typename inval_t, sc_in<inval_t>    , bits_number_getter<inval_t>::get());
BITS_NUMBER_GETTER_DEF(typename inval_t, sc_out<inval_t>   , bits_number_getter<inval_t>::get());
BITS_NUMBER_GETTER_DEF(typename inval_t, sc_inout<inval_t> , bits_number_getter<inval_t>::get());
BITS_NUMBER_GETTER_DEF(typename inval_t, sc_signal<inval_t>, bits_number_getter<inval_t>::get());
BITS_NUMBER_GETTER_DEF(int W, sc_int<W>, W);
BITS_NUMBER_GETTER_DEF(int W, sc_uint<W>, W);
BITS_NUMBER_GETTER_DEF(int W, sc_bigint<W>, W);
BITS_NUMBER_GETTER_DEF(int W, sc_biguint<W>, W);
BITS_NUMBER_GETTER_DEF(int W, sc_bv<W>, W);
BITS_NUMBER_GETTER_DEF(int W, sc_lv<W>, W);
#undef BITS_NUMBER_GETTER_DEF


template < typename T >
inline unsigned int get_bits_number (const T & object) {
    return bits_number_getter<T>::get();
}



struct sc_trace_file;


extern void sc_trace(sc_trace_file * tf, const signal2trace & t, const std::string & name);


template < class T > /*inline*/
void sc_trace(sc_trace_file * tf, const sc_in< T > & port, const std::string & name) {
    signal2trace t;
    t.inter = (const sc_interface *) &port;
    t.alias = alias();
    t.bit_size = get_bits_number(port);
    t.nomSig = name;
    sc_trace (tf, t, name);
}



template <class T> /*inline*/
void sc_trace(sc_trace_file * tf, const sc_out< T > & out, const std::string & name) {
    signal2trace t;
    t.inter = (const sc_interface *) &out;
    t.alias = alias();
    t.bit_size = get_bits_number(out);
    t.nomSig = name;
    sc_trace (tf, t, name);
}



template <typename T> /*inline*/
void sc_trace(sc_trace_file * tf, const sc_inout< T > & inout, const std::string & name) {
    signal2trace t;
    t.inter = (const sc_interface *) &inout;
    t.alias = alias();
    t.bit_size = get_bits_number(inout);
    t.nomSig = name;
    sc_trace (tf, t, name);
}



template <typename T> /*inline*/
void sc_trace( sc_trace_file * tf, const sc_signal< T > & signal, const std::string & name) {
    signal2trace t;
    t.inter = (const sc_interface *) &signal;
    t.alias = alias();
    t.bit_size = get_bits_number(signal);
    t.nomSig = name;
    sc_trace (tf, t, name);
}



#define DEF_SC_TRACE(T) /*inline \*/       \
    extern void sc_trace (sc_trace_file *, \
        const T & object,                  \
        const std::string &);

    DEF_SC_TRACE(bool)
    DEF_SC_TRACE(float)
    DEF_SC_TRACE(double)
    DEF_SC_TRACE(unsigned char)
    DEF_SC_TRACE(unsigned short)
    DEF_SC_TRACE(unsigned int)
    DEF_SC_TRACE(unsigned long)
    DEF_SC_TRACE(char)
    DEF_SC_TRACE(short)
    DEF_SC_TRACE(int)
    DEF_SC_TRACE(long)
    DEF_SC_TRACE(uint64)
    DEF_SC_TRACE(int64)
#undef DEF_SC_TRACE

} // end of sc_core namespace


#endif

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

