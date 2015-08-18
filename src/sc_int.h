/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_int.h                          |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_INT_H__
#define __SC_INT_H__

#include <data_field.h>
#include <sc_numrep.h>
#include <sc_bit.h>
#include <sc_logic.h>
#include <sc_bv.h>
#include <cstdlib>

// ----------------------------------------------------------------------------
//  CLASS : sc_int<W>
//
//  Template class sc_int<W> is the interface that the user sees. It is
//  derived from sc_int_base and most of its methods are just wrappers
//  that call the corresponding method in the parent class. Note that
//  the width of sc_int datatype is specified as a template parameter.
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"

namespace sc_dt {

typedef sc_int<1> sc_int_bit_ref;
typedef sc_int<1> sc_int_bit_ref_r;


#define MASK32(W) ((~ (const uint32)0) >> (sizeof (uint32) * 8 - W))
#define MASK64(W) ((~ (const uint64)0) >> (sizeof (uint64) * 8 - W))

template<int W> struct s_int_type { typedef int64 int_type; };

#define DECLAR_INT_TYPE(W) template<> struct s_int_type<W> { typedef smallest_int int_type; } // not declared as in8 because << operator threats like a character
DECLAR_INT_TYPE(1);
DECLAR_INT_TYPE(2);
DECLAR_INT_TYPE(3);
DECLAR_INT_TYPE(4);
DECLAR_INT_TYPE(5);
DECLAR_INT_TYPE(6);
DECLAR_INT_TYPE(7);
DECLAR_INT_TYPE(8);
#undef DECLAR_INT_TYPE

#define DECLAR_INT_TYPE(W) template<> struct s_int_type<W> { typedef int16 int_type; }
DECLAR_INT_TYPE( 9);
DECLAR_INT_TYPE(10);
DECLAR_INT_TYPE(11);
DECLAR_INT_TYPE(12);
DECLAR_INT_TYPE(13);
DECLAR_INT_TYPE(14);
DECLAR_INT_TYPE(15);
DECLAR_INT_TYPE(16);
#undef DECLAR_INT_TYPE

#define DECLAR_INT_TYPE(W) template<> struct s_int_type<W> { typedef int32 int_type; }
DECLAR_INT_TYPE(17);
DECLAR_INT_TYPE(18);
DECLAR_INT_TYPE(19);
DECLAR_INT_TYPE(20);
DECLAR_INT_TYPE(21);
DECLAR_INT_TYPE(22);
DECLAR_INT_TYPE(23);
DECLAR_INT_TYPE(24);
DECLAR_INT_TYPE(25);
DECLAR_INT_TYPE(26);
DECLAR_INT_TYPE(27);
DECLAR_INT_TYPE(28);
DECLAR_INT_TYPE(29);
DECLAR_INT_TYPE(30);
DECLAR_INT_TYPE(31);
DECLAR_INT_TYPE(32);
#undef DECLAR_INT_TYPE

// --------------------------------
// CLASS : sc_int_subref_r
//
// Used for range, concat functions

class sc_int_subref_r {

    template <int W> friend class sc_dt::sc_int;
    int left, right;
    
    public :
    int64 val;
    sc_int_subref_r(int64 val_, int left_, int right_) {
        val = val_; left = left_; right = right_;
    }
    
    inline int64 read () const { return val; }
    inline const sc_int_subref_r& operator | (const sc_int_subref_r &v) const {
        print_warning ();
        return *this;
    }

    private :
    void print_warning () const;

};

//
// to fix : propagation of the sign flag
//

class sc_int_subref_r;

template< int W /* = SC_INTWIDTH */>
class sc_int {

    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/
    typedef sc_int<W> this_type;
    typedef typename s_int_type<W>::int_type data_type;

    typedef data_type sc_int_subref;

    // internal
    union {
        val_field<W, (sizeof (data_type) * 8) - W,data_type> vf; /* To compute */
        data_type val;          /* To return an int reference (read function) */
    };


    /***********************/
    /*        L R M        */
    /***********************/
    public:
    sc_int()                  { val = 0; }
    sc_int (const char * a)   { val = 0; write (std::atoi (a)); }
    sc_int (unsigned short a) { val = 0; write (a); }
    sc_int (short a)          { val = 0; write (a); }
    sc_int (unsigned long a)  { val = 0; write (a); }
    sc_int (long a)           { val = 0; write (a); }
    sc_int (unsigned int a)   { val = 0; write (a); }
    sc_int (int a)            { val = 0; write (a); }
    sc_int (int64 a)          { val = 0; write (a); }
    sc_int (uint64 a)         { val = 0; write (a); }
    sc_int (double a)         { val = 0; write (a); }

    template <int W2> sc_int (const sc_int<W2> &val_) { val = 0; write (val_.read());}
    /* this template doesn't redefine default copy constructor of sc_int.
     * So, we define by this way 
     */

    sc_int (const sc_int &val_) { val = val_.val; }
    sc_int (const sc_int_subref_r & a) { val = 0; write (a); } 
    /* the user needs to cast explicitly result of range () method. */


    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/

    // read/write
    inline const data_type& read() const { return val; }
    inline void write(data_type val_)    { vf.valW = val_; }
    template <int W2> inline void write (const sc_int<W2> val_) { write (val_.read ()); }
    inline void write (const sc_int_subref_r& s) { write (s.read()); }

    /***********************/
    /*        L R M        */
    /***********************/

    // operators
    inline operator const data_type & () const {
        return read ();
    }

    template <typename T> inline sc_int& operator = (const T& val_) {
        write (val_);
        return *this;
    }

    inline sc_int & operator = (const sc_int_subref_r& a) {
        write (a);
        return *this;
    }

    // explicit conversions
    inline uint32 to_uint()   const { return val & MASK32(W); }
    inline int32  to_int()    const { return val & MASK32(W); }
    inline uint64 to_uint64() const { return val & MASK64(W); }
    inline int64  to_int64()  const { return val & MASK64(W); }

    // explicit conversion to character string 
    const sc_string to_string(sc_numrep numrep = SC_DEC) const {
        return sc_dt::to_string (val, W, numrep);
    }
    const sc_string to_dec() const { return to_string (SC_DEC); }
    const sc_string to_bin() const { return to_string (SC_BIN); }
    const sc_string to_oct() const { return to_string (SC_OCT); }
    const sc_string to_hex() const { return to_string (SC_HEX); }

    // arithmetic
#define DEFINE_OPERATOR(OPER)           \
    template <typename T>               \
    inline sc_int & operator OPER (T v) \
    { vf.valW OPER v; return *this; }

    DEFINE_OPERATOR(<<=)
    DEFINE_OPERATOR(>>=)
    DEFINE_OPERATOR(+=)
    DEFINE_OPERATOR(-=)
    DEFINE_OPERATOR(*=)
    DEFINE_OPERATOR(/=)
    DEFINE_OPERATOR(%=)
    DEFINE_OPERATOR(&=)
    DEFINE_OPERATOR(|=)
    DEFINE_OPERATOR(^=)
#undef DEFINE_OPERATOR

    inline sc_int_bit_ref & operator [] (int v) {
        return (vf.valW >> v) & 1;
    }
    inline sc_int_bit_ref_r & operator [] (int v) const {
        return (vf.valW >> v) & 1;
    }

    template <int W2> 
    inline sc_int<W + W2> operator , (const sc_int<W2> & b) const {
        sc_int<W + W2> res = read() << W2; res += b.read();
        return res;
    }

    inline sc_int<W + 1> operator , (bool b) const {
        sc_int<W + 1> res = read(); res <<= 1; res += b;
        return res;
    }

    template <int W2>
    inline sc_int<W2> operator , (const sc_int_subref_r &v) const {
        std::cerr << "Warning : \n";
        return sc_int<W2> (v.read());
    }

    inline sc_int_subref range (int left, int right) {
        return (data_type)((data_type) (((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) & val) >> right;
    }

    inline sc_int_subref_r range (int left, int right) const {
        return sc_int_subref_r (((data_type) (((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) & val) >> right, left, right);
    }
#if 0
    std::cerr << "range(" << left << "," << right << ")\n";
    std::cerr << "val = " << val << "\n";
    std::cerr << "~0 >> " << (sizeof (data_type) * 8 - left - 1) << " = " << (data_type) (((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) << "\n";
    std::cerr <<  ((data_type) ((((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) & val) >> right) << "\n";
    std::cerr << "data_type = " << sizeof (data_type) << "\n";
#endif

};

inline std::ostream & operator << (std::ostream &o, const sc_int_subref_r & s) {
    return o << s.val;
}

#undef MASK32
#undef MASK64

} /* end of sc_dt namespace */

#endif /* __SC_INT_H__ */

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

