/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_uint.h                         |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_UINT_H__
#define __SC_UINT_H__

#include <data_field.h>
#include <sc_numrep.h>
#include <sc_bit.h>
#include <sc_logic.h>
#include <sc_bv.h>
#include <cstdlib>

// ----------------------------------------------------------------------------
//  CLASS : sc_uint<W>
//
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"

namespace sc_dt {

typedef sc_uint<1> sc_uint_bit_ref;
typedef sc_uint<1> sc_uint_bit_ref_r;


// --------------------------------
// CLASS : sc_uint_subref_r
//
// Used for range, concat functions
// --------------------------------

class sc_uint_subref_r {
    template <int W> friend class sc_dt::sc_uint;
    int left, right;
    public :
    uint64 val;
    sc_uint_subref_r(uint64 val_, int left_, int right_) {
        val = val_;
        left = left_;
        right = right_;
    }

    public:
    inline uint64 read() const {
        return val;
    }

    inline const sc_uint_subref_r & operator | (const sc_uint_subref_r & v) const {
        print_warning();
        return *this;
    }

    private:
    void print_warning() const;

};


#define MASK32(W) ((~ (const uint32)0) >> (sizeof(uint32) * 8 - W))
#define MASK64(W) ((~ (const uint64)0) >> (sizeof(uint64) * 8 - W))


template<int W> struct s_uint_type {
    typedef uint64 uint_type;
};

#define DECLAR_UINT_TYPE(W) template<> struct s_uint_type<W> { typedef smallest_uint uint_type; }// not declared as uint16 because << operator threats like a character
DECLAR_UINT_TYPE(1);
DECLAR_UINT_TYPE(2);
DECLAR_UINT_TYPE(3);
DECLAR_UINT_TYPE(4);
DECLAR_UINT_TYPE(5);
DECLAR_UINT_TYPE(6);
DECLAR_UINT_TYPE(7);
DECLAR_UINT_TYPE(8);
#undef DECLAR_UINT_TYPE

#define DECLAR_UINT_TYPE(W) template<> struct s_uint_type<W> { typedef uint16 uint_type; }
DECLAR_UINT_TYPE(9);
DECLAR_UINT_TYPE(10);
DECLAR_UINT_TYPE(11);
DECLAR_UINT_TYPE(12);
DECLAR_UINT_TYPE(13);
DECLAR_UINT_TYPE(14);
DECLAR_UINT_TYPE(15);
DECLAR_UINT_TYPE(16);
#undef DECLAR_UINT_TYPE

#define DECLAR_UINT_TYPE(W) template<> struct s_uint_type<W> { typedef uint32 uint_type; }
DECLAR_UINT_TYPE(17);
DECLAR_UINT_TYPE(18);
DECLAR_UINT_TYPE(19);
DECLAR_UINT_TYPE(20);
DECLAR_UINT_TYPE(21);
DECLAR_UINT_TYPE(22);
DECLAR_UINT_TYPE(23);
DECLAR_UINT_TYPE(24);
DECLAR_UINT_TYPE(25);
DECLAR_UINT_TYPE(26);
DECLAR_UINT_TYPE(27);
DECLAR_UINT_TYPE(28);
DECLAR_UINT_TYPE(29);
DECLAR_UINT_TYPE(30);
DECLAR_UINT_TYPE(31);
DECLAR_UINT_TYPE(32);
#undef DECLAR_UINT_TYPE


class sc_uint_subref_r;


/* width of data type = SC_INTWIDTH */
template<int W>
class sc_uint {

    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/
    typedef sc_uint<W> this_type;
    typedef typename s_uint_type<W>::uint_type data_type;
    typedef data_type sc_uint_subref;
    //typedef data_type sc_uint_subref_r; /* removed since "operator ," was going wrong */


    // internal
    union {
        val_field<W, (sizeof(data_type) * 8) - W, data_type> vf; /* To compute */
        data_type val;          /* To return an int reference (read function) */
        // data_type valW: W; /* works with little endianess only */
        // bool valB[W];  /* removed since 1 bool takes 1 byte */
    };

    /***********************/
    /*        L R M        */
    /***********************/
    public:
    sc_uint()                 { val = 0; }
    //  sc_uint(data_type val_)  { val = 0; write (val_); }
    sc_uint(const char * a)   { val = 0; write(std::atoi(a)); }
    sc_uint(unsigned short a) { val = 0; write(a); }
    sc_uint(short a)          { val = 0; write(a); }
    sc_uint(unsigned long a)  { val = 0; write(a); }
    sc_uint(long a)           { val = 0; write(a); }
    sc_uint(unsigned int a)   { val = 0; write(a); }
    sc_uint(int a)            { val = 0; write(a); }
    sc_uint(int64 a)          { val = 0; write(a); }
    sc_uint(uint64 a)         { val = 0; write(a); }
    sc_uint(double a)         { val = 0; write(a); }


    template <int W2> sc_uint(const sc_uint<W2> & val_) {
        val = 0;
        write(val_.read());
    }
    
    /* this template doesn't redefine default copy constructor of sc_uint.
     * So, we define by this way 
     */
    sc_uint(const sc_uint & val_)          { val = 0; write(val_.read()); }
    sc_uint(const sc_uint_subref_r & val_) { val = 0; write(val_.read()); } 
    /* the user needs to cast explicitly result of range () method. */

    /***********************/
    /* SYSTEMCASS SPECIFIC */
    /***********************/
    // read/write
    inline const data_type & read() const {
        return val;
    }

    inline void write(data_type val_) {
        vf.valW = val_;
    }

    template <int W2> inline void write(const sc_uint<W2> val_) {
        write(val_.read());
    }

    // inline void write(const sc_uint<W> val_) {
    //     write(val_.read());
    // }
    
    inline void write(const sc_uint_subref_r & s) {
        write(s.read());
    }


    /***********************/
    /*        L R M        */
    /***********************/
    // operators
    inline operator const data_type & () const { 
        return read();
    }

    // inline void write(uint64 val_) {
    //     val = val_ & MASK64(W);
    // }
    // inline void write(unsigned int val_) {
    //     val = val_ & MASK32(W);
    // }
    
    template < typename T > inline sc_uint & operator = (const T & val_) {
        write(val_);
        return *this;
    }

    inline sc_uint & operator = (const sc_uint_subref_r & a) {
        write(a);
        return *this;
    }

    // explicit conversions
    inline uint32 to_uint()   const { return val & MASK32(W); }
    inline int32  to_int()    const { return val & MASK32(W); }
    inline uint64 to_uint64() const { return val & MASK64(W); }
    inline int64  to_int64()  const { return val & MASK64(W); }

    // explicit conversion to character string 
    const sc_string to_string(sc_numrep numrep = SC_DEC) const {
        return sc_dt::to_string(val, W, numrep);
    }
    const sc_string to_dec() const { return to_string(SC_DEC); }
    const sc_string to_bin() const { return to_string(SC_BIN); }
    const sc_string to_oct() const { return to_string(SC_OCT); }
    const sc_string to_hex() const { return to_string(SC_HEX); }


    // arithmetic
#define DEFINE_OPERATOR(OPER)            \
    template < typename T >              \
    inline sc_uint & operator OPER (T v) \
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


    inline sc_uint_bit_ref operator [] (int v) {
        return (vf.valW >> v) & 1;
    }

    inline sc_uint_bit_ref_r operator [] (int v) const {
        return (vf.valW >> v) & 1;
    }

    template <int W2> 
    inline sc_uint<W + W2> operator , (const sc_uint<W2> & b) const {
        sc_uint<W + W2> res = read() << W2;
        res += b.read();
        return res;
    }

    inline sc_uint<W + 1> operator , (bool b) const {
        sc_uint<W + 1> res = read();
        res <<= 1;
        res += b;
        return res;
    }

    template <int W2>
    inline sc_uint<W2> operator , (const sc_uint_subref_r & v) const {
        std::cerr << "Warning : \n"; // ??
        return sc_uint<W2> (v.read());
    }

    inline sc_uint_subref range(int left, int right) {
        return (data_type) ((data_type) (((data_type) ~(0)) >> (sizeof(data_type) * 8 - left - 1)) & val) >> right;
    }

    inline sc_uint_subref_r range(int left, int right) const {
        return sc_uint_subref_r (((data_type) (((data_type) ~(0)) >> (sizeof(data_type) * 8 - left - 1)) & val) >> right, left, right);
    }

#if 0
    std::cerr << "range(" << left << "," << right << ")\n";
    std::cerr << "val = " << val << "\n";
    std::cerr << "~0 >> " << (sizeof (data_type) * 8 - left - 1) << " = " << (data_type) (((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) << "\n";
    std::cerr <<  ((data_type) ((((data_type)~(0)) >> (sizeof (data_type) * 8 - left - 1)) & val) >> right) << "\n";
    std::cerr << "data_type = " << sizeof (data_type) << "\n";
#endif
    
#undef MASK32
#undef MASK64

}; 

//
// no sign flag to threat

inline std::ostream & operator << (std::ostream & o, const sc_uint_subref_r & s) {
    return o << s.val;
}


} /* end of sc_dt namespace */

#endif /* __SC_UINT_H__ */

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

