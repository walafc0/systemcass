/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_bit.h                          |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   14_04_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_BIT_H__
#define __SC_BIT_H__

#include "sc_nbdefs.h"
#include "sc_fwd.h"
#include "sc_logic.h"
#include "sc_string.h"
#include "sc_numrep.h"

// ----------------------------------------------------------------------------
//  CLASS : sc_bit
//
// ----------------------------------------------------------------------------

namespace sc_dt {

class sc_bit {
    
    /////////////////////////
    ///// SYSTEMCASS SPECIFIC
    bool val;
    /////////////////////////

    public:
    // constructors  & destructor 
    sc_bit()                            { val = false; }
    explicit sc_bit(bool a)             { val = a; }
    explicit sc_bit(int a)              { val = a > 0; }
    explicit sc_bit(char a)             { val = (a == '1') ? true : false; }
    explicit sc_bit(const sc_logic & a) { val = a.to_bool(); }
    ~sc_bit()                           {}
    // copy constructor 
    sc_bit(const sc_bit & a)            { val = a; }
    // assignment operators 
    sc_bit & operator = (const sc_bit & b)   { val = b.to_bool(); return *this; }
    sc_bit & operator = (int b)              { val = b > 0; return *this; }
    sc_bit & operator = (bool b)             { val = b; return *this; }
    sc_bit & operator = (char b)             { val = (b == '1') ? true : false; return *this; }
    sc_bit & operator = (const sc_logic & b) { val = b.to_bool(); return *this; }

    // bitwise assignment operators 
    sc_bit & operator &= (const sc_bit & b)  { val &= b.to_bool(); return *this; }
    sc_bit & operator &= (int b)             { val &= b > 0; return *this; }
    sc_bit & operator &= (bool b)            { val &= b; return *this; }
    sc_bit & operator &= (char b)            { val &= (b == '1') ? true : false; return *this; }
    sc_bit & operator |= (const sc_bit & b)  { val |= b.val; return *this; }
    sc_bit & operator |= (int b)             { val |= b > 0; return *this; }
    sc_bit & operator |= (bool b)            { val |= b; return *this; }
    sc_bit & operator |= (char b)            { val |= (b == '1') ? true : false; return *this; }
    sc_bit & operator ^= (const sc_bit & b)  { val ^= b.val; return *this; }
    sc_bit & operator ^= (int b)             { val ^= b > 0; return *this; }
    sc_bit & operator ^= (bool b)            { val ^= b; return *this; }
    sc_bit & operator ^= (char b)            { val ^= (b == '1') ? true : false; return *this; }

    // implicit conversion to bool 
    operator bool () const   { return val;  }
    bool operator ! () const { return !val; }

    // explicit conversion to character string 
    const sc_string to_string(sc_numrep numrep = SC_DEC ) const  {
        return sc_string ((val) ? "1" : "0");
    }
    const sc_string to_dec() const { return to_string (SC_DEC); }
    const sc_string to_bin() const { return to_string (SC_BIN); }
    const sc_string to_oct() const { return to_string (SC_OCT); }
    const sc_string to_hex() const { return to_string (SC_HEX); }

    // others explicit conversions 
    bool to_bool() const { return val;  }
    char to_char() const { return (val) ? '1' : '0'; }

    // relational operators and functions 
    friend bool operator == (const sc_bit & a, const sc_bit & b) { return a == b; }
    friend bool operator == (const sc_bit & a, int b);
    friend bool operator == (const sc_bit & a, bool b);
    friend bool operator == (const sc_bit & a, char b);
    friend bool operator == (int a, const sc_bit & b);
    friend bool operator == (bool a, const sc_bit & b);
    friend bool operator == (char a, const sc_bit & b);
    friend bool equal (const sc_bit & a, const sc_bit & b);
    friend bool equal (const sc_bit & a, int b);
    friend bool equal (const sc_bit & a, bool b);
    friend bool equal (const sc_bit & a, char b);
    friend bool equal (int a, const sc_bit & b);
    friend bool equal (bool a, const sc_bit & b);
    friend bool equal (char a, const sc_bit & b);
    friend bool operator != (const sc_bit & a, const sc_bit & b);
    friend bool operator != (const sc_bit & a, int b);
    friend bool operator != (const sc_bit & a, bool b);
    friend bool operator != (const sc_bit & a, char b);
    friend bool operator != (int a, const sc_bit & b);
    friend bool operator != (bool a, const sc_bit & b);
    friend bool operator != (char a, const sc_bit & b);
    friend bool not_equal (const sc_bit & a, const sc_bit & b);
    friend bool not_equal (const sc_bit & a, int b);
    friend bool not_equal (const sc_bit & a, bool b);
    friend bool not_equal (const sc_bit & a, char b);
    friend bool not_equal (int a, const sc_bit & b);
    friend bool not_equal (bool a, const sc_bit & b);
    friend bool not_equal (char a, const sc_bit & b);
    // bitwise complement 
    friend const sc_bit operator ~ (const sc_bit & a);
    sc_bit & b_not();
    friend void b_not (sc_bit & r, const sc_bit & a);
    friend const sc_bit b_not (const sc_bit & a);
    // bitwise or 
    friend const sc_bit operator | (const sc_bit & a, const sc_bit & b);
    friend const sc_bit operator | (const sc_bit & a, int b);
    friend const sc_bit operator | (const sc_bit & a, bool b);
    friend const sc_bit operator | (const sc_bit & a, char b);
    friend const sc_bit operator | (int a, const sc_bit & b);
    friend const sc_bit operator | (bool a, const sc_bit & b);
    friend const sc_bit operator | (char a, const sc_bit & b);
    friend const sc_bit b_or (const sc_bit & a, const sc_bit & b);
    friend const sc_bit b_or (const sc_bit & a, int b);
    friend const sc_bit b_or (const sc_bit & a, bool b);
    friend const sc_bit b_or (const sc_bit & a, char b);
    friend const sc_bit b_or (int a, const sc_bit & b);
    friend const sc_bit b_or (bool a, const sc_bit & b);

    friend const sc_bit b_or (char a, const sc_bit & b);
    friend void b_or (sc_bit & r, const sc_bit & a, const sc_bit & b);
    friend void b_or (sc_bit & r, const sc_bit & a, int b);
    friend void b_or (sc_bit & r, const sc_bit & a, bool b);
    friend void b_or (sc_bit & r, const sc_bit & a, char b);
    friend void b_or (sc_bit & r, int a, const sc_bit & b);
    friend void b_or (sc_bit & r, bool a, const sc_bit & b);
    friend void b_or (sc_bit & r, char a, const sc_bit & b);
    // bitwise and 
    friend const sc_bit operator & (const sc_bit & a, const sc_bit & b);
    friend const sc_bit operator & (const sc_bit & a, int b);
    friend const sc_bit operator & (const sc_bit & a, bool b);
    friend const sc_bit operator & (const sc_bit & a, char b);
    friend const sc_bit operator & (int a, const sc_bit & b);
    friend const sc_bit operator & (bool a, const sc_bit & b);
    friend const sc_bit operator & (char a, const sc_bit & b);
    friend const sc_bit b_and (const sc_bit & a, const sc_bit & b);
    friend const sc_bit b_and (const sc_bit & a, int b);
    friend const sc_bit b_and (const sc_bit & a, bool b);
    friend const sc_bit b_and (const sc_bit & a, char b);
    friend const sc_bit b_and (int a, const sc_bit & b);
    friend const sc_bit b_and (bool a, const sc_bit & b);
    friend const sc_bit b_and (char a, const sc_bit & b);
    friend void b_and (sc_bit & r, const sc_bit & a, const sc_bit & b);
    friend void b_and (sc_bit & r, const sc_bit & a, int b);
    friend void b_and (sc_bit & r, const sc_bit & a, bool b);
    friend void b_and (sc_bit & r, const sc_bit & a, char b);
    friend void b_and (sc_bit & r, int a, const sc_bit & b);
    friend void b_and (sc_bit & r, bool a, const sc_bit & b);
    friend void b_and (sc_bit & r, char a, const sc_bit & b);
    // bitwise exor 
    friend const sc_bit operator ^ (const sc_bit & a, const sc_bit & b);
    friend const sc_bit operator ^ (const sc_bit & a, int b);
    friend const sc_bit operator ^ (const sc_bit & a, bool b);
    friend const sc_bit operator ^ (const sc_bit & a, char b);
    friend const sc_bit operator ^ (int a, const sc_bit & b);

    friend const sc_bit operator ^ (bool a, const sc_bit & b);
    friend const sc_bit operator ^ (char a, const sc_bit & b);
    friend const sc_bit b_xor (const sc_bit & a, const sc_bit & b);
    friend const sc_bit b_xor (const sc_bit & a, int b);
    friend const sc_bit b_xor (const sc_bit & a, bool b);
    friend const sc_bit b_xor (const sc_bit & a, char b);
    friend const sc_bit b_xor (int a, const sc_bit & b);
    friend const sc_bit b_xor (bool a, const sc_bit & b);
    friend const sc_bit b_xor (char a, const sc_bit & b);
    friend void b_xor (sc_bit & r, const sc_bit & a, const sc_bit & b);
    friend void b_xor (sc_bit & r, const sc_bit & a, int b);
    friend void b_xor (sc_bit & r, const sc_bit & a, bool b);
    friend void b_xor (sc_bit & r, const sc_bit & a, char b);
    friend void b_xor (sc_bit & r, int a, const sc_bit & b);
    friend void b_xor (sc_bit & r, bool a, const sc_bit & b);
    friend void b_xor (sc_bit & r, char a, const sc_bit & b);

};

} /* end of sc_dt namespace */

#endif /* __SC_BIT_H__ */

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

