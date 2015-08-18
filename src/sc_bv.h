/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_bv.h                           |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   14_04_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_BV_H__
#define __SC_BV_H__

// ----------------------------------------------------------------------------
//  CLASS : sc_bv
//
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"
#include "sc_logic.h"
#include "sc_unsigned.h"
#include "sc_signed.h"
#include "sc_uint.h"
#include "sc_int.h"

namespace sc_dt {

class sc_bv_base {
    public:
    // constructors 
    sc_bv_base (const char * a);
    sc_bv_base (const char * a, int length_);
    template < class X > sc_bv_base (const sc_bv_base & a);
    virtual ~ sc_bv_base ();
    // assignment operators 
    template < class X > sc_bv_base & operator = (const sc_bv_base & a);
    sc_bv_base & operator = (const char *a);
    sc_bv_base & operator = (const bool * a);
    sc_bv_base & operator = (const sc_logic * a);
    sc_bv_base & operator = (const sc_unsigned & a);
    sc_bv_base & operator = (const sc_signed & a);
    sc_bv_base & operator = (unsigned long a);
    sc_bv_base & operator = (long a);
    sc_bv_base & operator = (unsigned int a);
    sc_bv_base & operator = (int a);
    sc_bv_base & operator = (uint64 a);
    sc_bv_base & operator = (int64 a);

    // methods 
    int length () const;
    bool is_01 () const;
};


template < int W >
class sc_bv : public sc_bv_base {
    public:
    // constructors 
    sc_bv ();
    explicit sc_bv (bool init_value);
    explicit sc_bv (char init_value);
    sc_bv (const char *a);
    sc_bv (const bool * a);
    sc_bv (const sc_logic * a);
    sc_bv (const sc_unsigned & a);
    sc_bv (const sc_signed & a);
    sc_bv (unsigned long a);
    sc_bv (long a);
    sc_bv (unsigned int a);
    sc_bv (int a);
    sc_bv (uint64 a);
    sc_bv (int64 a);
    sc_bv (const sc_bv_base & a);
    sc_bv (const sc_bv < W > & a);
    // assignment operators 
    template < class X > sc_bv < W > & operator = (const sc_bv_base & a);
    sc_bv < W > & operator = (const sc_bv < W > &a);
    sc_bv < W > & operator = (const char *a);
    sc_bv < W > & operator = (const bool * a);
    sc_bv < W > & operator = (const sc_logic * a);
    sc_bv < W > & operator = (const sc_unsigned & a);
    sc_bv < W > & operator = (const sc_signed & a);
    sc_bv < W > & operator = (unsigned long a);
    sc_bv < W > & operator = (long a);
    sc_bv < W > & operator = (unsigned int a);
    sc_bv < W > & operator = (int a);
    sc_bv < W > & operator = (uint64 a);
    sc_bv < W > & operator = (int64 a);
};

} /* end of sc_dt namespace */

#endif /* __SC_BV_H__ */

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

