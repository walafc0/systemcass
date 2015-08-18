/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_lv.h                           |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   19_04_2005                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_LV_H__
#define __SC_LV_H__

// ----------------------------------------------------------------------------
//  CLASS : sc_lv
//
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"
#include "sc_logic.h"
#include "sc_unsigned.h"
#include "sc_signed.h"
#include "sc_uint.h"
#include "sc_int.h"


namespace sc_dt {

template<int W> struct s_lv_type { typedef uint64 lv_type; };

#define DECLAR_LV_TYPE(W) template<> struct s_lv_type<W> { typedef smallest_uint lv_type; }// not declared as uint16 because << operator threats like a character
DECLAR_LV_TYPE(1);
DECLAR_LV_TYPE(2);
DECLAR_LV_TYPE(3);
DECLAR_LV_TYPE(4);
DECLAR_LV_TYPE(5);
DECLAR_LV_TYPE(6);
DECLAR_LV_TYPE(7);
DECLAR_LV_TYPE(8);
#undef DECLAR_LV_TYPE

#define DECLAR_LV_TYPE(W) template<> struct s_lv_type<W> { typedef uint16 lv_type; }
DECLAR_LV_TYPE(9);
DECLAR_LV_TYPE(10);
DECLAR_LV_TYPE(11);
DECLAR_LV_TYPE(12);
DECLAR_LV_TYPE(13);
DECLAR_LV_TYPE(14);
DECLAR_LV_TYPE(15);
DECLAR_LV_TYPE(16);
#undef DECLAR_LV_TYPE

#define DECLAR_LV_TYPE(W) template<> struct s_lv_type<W> { typedef uint32 lv_type; }
DECLAR_LV_TYPE(17);
DECLAR_LV_TYPE(18);
DECLAR_LV_TYPE(19);
DECLAR_LV_TYPE(20);
DECLAR_LV_TYPE(21);
DECLAR_LV_TYPE(22);
DECLAR_LV_TYPE(23);
DECLAR_LV_TYPE(24);
DECLAR_LV_TYPE(25);
DECLAR_LV_TYPE(26);
DECLAR_LV_TYPE(27);
DECLAR_LV_TYPE(28);
DECLAR_LV_TYPE(29);
DECLAR_LV_TYPE(30);
DECLAR_LV_TYPE(31);
DECLAR_LV_TYPE(32);
#undef DECLAR_LV_TYPE


class sc_lv_base {

    public:
    // constructors 
    sc_lv_base() {} // SYSTEMCASS SPECIFIC
    sc_lv_base(const char * a);
    sc_lv_base(const char * a, int length_);
    template < class X > sc_lv_base (const sc_lv_base & a);
    virtual ~sc_lv_base() {}
    // assignment operators 
    template < class X > sc_lv_base & operator = (const sc_lv_base & a);
    sc_lv_base & operator = (const char * a);
    sc_lv_base & operator = (const bool * a);
    sc_lv_base & operator = (const sc_logic * a);
    sc_lv_base & operator = (const sc_unsigned & a);
    sc_lv_base & operator = (const sc_signed & a);
    sc_lv_base & operator = (unsigned long a);
    sc_lv_base & operator = (long a);
    sc_lv_base & operator = (unsigned int a);
    sc_lv_base & operator = (int a);
    sc_lv_base & operator = (uint64 a);
    sc_lv_base & operator = (int64 a);

    // methods 
    int length() const;
    bool is_01() const;
};


template < int W > class sc_lv : public sc_lv_base {

    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/
    
    typedef sc_lv<W> this_type;
    typedef typename s_lv_type<W>::lv_type data_type;
    data_type val;

    public:
    // constructors 
    sc_lv() { val = 0; }
    explicit sc_lv(bool init_value) { val = 0; }
    explicit sc_lv (char init_value) { val = 0; }
    sc_lv(const char * a)        { val = 0; write(atoi(a)); }
    sc_lv(const bool * a)        { val = 0; write(a); }
    sc_lv(const sc_logic * a)    { val = 0; write(a); }
    sc_lv(const sc_unsigned & a) { val = 0; write(a); }
    sc_lv(const sc_signed & a)   { val = 0; write(a); }
    sc_lv(unsigned long a)       { val = 0; write(a); }  
    sc_lv(long a)                { val = 0; write(a); }
    sc_lv(unsigned int a)        { val = 0; write(a); }
    sc_lv(int a)                 { val = 0; write(a); }
    sc_lv(uint64 a)              { val = 0; write(a); }
    sc_lv(int64 a)               { val = 0; write(a); }
    sc_lv(const sc_lv_base & a)  { val = 0; write(a); }
    sc_lv(const sc_lv < W > & a) { val = 0; write(a); }
    
    // assignment operators 
#ifdef LRM
    template < class X > sc_lv < W > & operator = (const sc_lv_base & a);
    sc_lv < W > & operator = (const sc_lv < W > &a);
    sc_lv < W > & operator = (const char *a);
    sc_lv < W > & operator = (const bool * a);
    sc_lv < W > & operator = (const sc_logic * a);
    sc_lv < W > & operator = (const sc_unsigned & a);
    sc_lv < W > & operator = (const sc_signed & a);
#endif // LRM

    template <typename T> inline sc_lv& operator = (const T& val_) {
        write (val_);
        return *this;
    }
    sc_lv < W > & operator = (unsigned long a);
    sc_lv < W > & operator = (long a);
    sc_lv < W > & operator = (unsigned int a);
    sc_lv < W > & operator = (int a) { val = a; return *this; }
    sc_lv < W > & operator = (uint64 a);
    sc_lv < W > & operator = (int64 a);

    
    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/
    // read/write
    inline void write(data_type val_) {
        val = val_;
    }
    template <int W2> inline void write (const sc_lv<W2> val_) {
        write (val_.read());
    }

};

} /* end of sc_dt namespace */

#endif /* __SC_LV_H__ */

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

