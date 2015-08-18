/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_biguint.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_BIGUINT_H__
#define __SC_BIGUINT_H__

// ----------------------------------------------------------------------------
//  CLASS : sc_biguint<W>
//
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"

namespace sc_dt {

#define MASK32(W) ((~ (const uint32)0) >> (sizeof (uint32) * 8 - W))
#define MASK64(W) ((~ (const uint64)0) >> (sizeof (uint64) * 8 - W))

template<int W> struct s_biguint_type { typedef uint64 uint_type;};

#define DECLAR_BIGUINT_TYPE(W) template<> struct s_biguint_type<W> { typedef uint16 uint_type; }// not declared as uint16 because << operator threats like a character
DECLAR_BIGUINT_TYPE(1);
DECLAR_BIGUINT_TYPE(2);
DECLAR_BIGUINT_TYPE(3);
DECLAR_BIGUINT_TYPE(4);
DECLAR_BIGUINT_TYPE(5);
DECLAR_BIGUINT_TYPE(6);
DECLAR_BIGUINT_TYPE(7);
DECLAR_BIGUINT_TYPE(8);
#undef DECLAR_BIGUINT_TYPE

#define DECLAR_BIGUINT_TYPE(W) template<> struct s_biguint_type<W> { typedef uint16 uint_type; }
DECLAR_BIGUINT_TYPE(9);
DECLAR_BIGUINT_TYPE(10);
DECLAR_BIGUINT_TYPE(11);
DECLAR_BIGUINT_TYPE(12);
DECLAR_BIGUINT_TYPE(13);
DECLAR_BIGUINT_TYPE(14);
DECLAR_BIGUINT_TYPE(15);
DECLAR_BIGUINT_TYPE(16);
#undef DECLAR_BIGUINT_TYPE

#define DECLAR_BIGUINT_TYPE(W) template<> struct s_biguint_type<W> { typedef uint32 uint_type; }
DECLAR_BIGUINT_TYPE(17);
DECLAR_BIGUINT_TYPE(18);
DECLAR_BIGUINT_TYPE(19);
DECLAR_BIGUINT_TYPE(20);
DECLAR_BIGUINT_TYPE(21);
DECLAR_BIGUINT_TYPE(22);
DECLAR_BIGUINT_TYPE(23);
DECLAR_BIGUINT_TYPE(24);
DECLAR_BIGUINT_TYPE(25);
DECLAR_BIGUINT_TYPE(26);
DECLAR_BIGUINT_TYPE(27);
DECLAR_BIGUINT_TYPE(28);
DECLAR_BIGUINT_TYPE(29);
DECLAR_BIGUINT_TYPE(30);
DECLAR_BIGUINT_TYPE(31);
DECLAR_BIGUINT_TYPE(32);
#undef DECLAR_BIGUINT_TYPE


template< int W /* = SC_INTWIDTH */>
class sc_biguint {
    typedef sc_biguint<W> this_type;
    typedef typename s_int_type<W>::int_type data_type;

    typedef data_type sc_uint_subref;
    typedef data_type sc_uint_subref_r;

    union {
        data_type valW:W;
        bool      valB[W];
        data_type val;
    };
    
    void check () {
        if (W > 64) {
            std::cerr << "sc_biguint with W > 64 is not supported.\n";
            exit (20040528);
        }
    }

    public:
    
    sc_biguint() {
        check ();
        val = 0;
    }

    sc_biguint(data_type val_) {
        check ();
        val = 0;
        write (val_);
    }

    template <int W2> explicit sc_biguint (const sc_biguint<W2> & val_) {
        write (val_.read());
    }

    inline const data_type & read() const {
        return val;
    }

    inline operator const data_type & () const {
        return read ();
    }

    inline void write(data_type val_) {
        valW = val_;
    }

    template <int W2> inline void write (const sc_biguint<W2> val_) {
        write (val_.read ());
    }

    inline void write (const sc_biguint<W> val_) {
        write (val_.read());
    }

    template <typename T> inline sc_biguint & operator = (const T& val_) {
        write (val_);
        return *this;
    }

    inline uint32 to_uint()   const { return val & MASK32(W); }
    inline int32  to_int()    const { return val & MASK32(W); }
    inline uint64 to_uint64() const { return val & MASK64(W); }
    inline int64  to_int64()  const { return val & MASK64(W); }

    template <typename T>
    inline sc_biguint & operator <<= (T v) {
        valW <<= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator >>= (T v) {
        valW >>= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator += (T v) {
        valW += v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator -= (T v) {
        valW -= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator *= (T v) {
        valW *= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator /= (T v) {
        valW /= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator %= (T v) {
        valW %= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator &= (T v) {
        valW &= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator |= (T v) {
        valW |= v;
        return *this;
    }

    template <typename T>
    inline sc_biguint & operator ^= (T v) {
        valW ^= v;
        return *this;
    }

    inline sc_uint_bit_ref &   operator [] (int v) { return valB[v]; }
    inline sc_uint_bit_ref_r & operator [] (int v) const { return valB[v]; }

    inline sc_uint_subref range (int left, int right) {
        return (((~0) >> (sizeof(data_type) * 8 - left)) & val) >> right;
    }

    inline sc_uint_subref_r range (int left, int right) const {
        return (((~0) >> (sizeof(data_type) * 8 - left)) & val) >> right;
    }

};

} /* end of sc_dt namespace */

#endif /* __SC_BIGUINT_H__ */

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

