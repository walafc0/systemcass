/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_logic.h                        |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   14_04_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_LOGIC_H__
#define __SC_LOGIC_H__

// ----------------------------------------------------------------------------
//  CLASS : sc_logic
//
// ----------------------------------------------------------------------------

#include "sc_nbdefs.h"
#include "sc_fwd.h"

namespace sc_dt {

// ----------------------------------------------------------------------------
//  ENUM : sc_logic_value_t
//
//  Enumeration of logic values.
// ----------------------------------------------------------------------------

enum sc_logic_value_t {
    Log_0 = 0,
    Log_1,
    Log_Z,
    Log_X
};


class sc_logic {
    public:
    // constructors & destructor 
    sc_logic();
    sc_logic(const sc_logic & a);
    sc_logic(sc_logic_value_t v);
    explicit sc_logic(bool a);
    explicit sc_logic(char a);
    explicit sc_logic(int a);
    explicit sc_logic(const sc_bit & a);
    ~sc_logic();
    // assignment operators 
    sc_logic & operator = (const sc_logic & a);
    sc_logic & operator = (sc_logic_value_t v);
    sc_logic & operator = (bool a);
    sc_logic & operator = (char a);
    sc_logic & operator = (int a);
    sc_logic & operator = (const sc_bit & a);
    // bitwise assignment operators 
    sc_logic & operator &= (const sc_logic & b);
    sc_logic & operator &= (sc_logic_value_t v);
    sc_logic & operator &= (bool b);
    sc_logic & operator &= (char b);
    sc_logic & operator &= (int b);
    sc_logic & operator |= (const sc_logic & b);
    sc_logic & operator |= (sc_logic_value_t v);
    sc_logic & operator |= (bool b);
    sc_logic & operator |= (char b);
    sc_logic & operator |= (int b);
    sc_logic & operator ^= (const sc_logic & b);
    sc_logic & operator ^= (sc_logic_value_t v);
    sc_logic & operator ^= (bool b);
    sc_logic & operator ^= (char b);
    sc_logic & operator ^= (int b);
    // bitwise complement 
    const sc_logic operator ~ () const;
    sc_logic & b_not();
    // bitwise and 
    friend const sc_logic operator & (const sc_logic & a, const sc_logic & b);
    friend const sc_logic operator & (const sc_logic & a, sc_logic_value_t b);
    friend const sc_logic operator & (const sc_logic & a, bool b);
    friend const sc_logic operator & (const sc_logic & a, char b);
    friend const sc_logic operator & (const sc_logic & a, int b);
    friend const sc_logic operator & (sc_logic_value_t a, const sc_logic & b);
    friend const sc_logic operator & (bool a, const sc_logic & b);
    friend const sc_logic operator & (char a, const sc_logic & b);
    friend const sc_logic operator & (int a, const sc_logic & b);
    // bitwise or 
    friend const sc_logic operator | (const sc_logic & a, const sc_logic & b);
    friend const sc_logic operator | (const sc_logic & a, sc_logic_value_t b);
    friend const sc_logic operator | (const sc_logic & a, bool b);
    friend const sc_logic operator | (const sc_logic & a, char b);
    friend const sc_logic operator | (const sc_logic & a, int b);
    friend const sc_logic operator | (sc_logic_value_t a, const sc_logic & b);
    friend const sc_logic operator | (bool a, const sc_logic & b);
    friend const sc_logic operator | (char a, const sc_logic & b);
    friend const sc_logic operator | (int a, const sc_logic & b);
    // bitwise xor 
    friend const sc_logic operator ^ (const sc_logic & a, const sc_logic & b);
    friend const sc_logic operator ^ (const sc_logic & a, sc_logic_value_t b);
    friend const sc_logic operator ^ (const sc_logic & a, bool b);
    friend const sc_logic operator ^ (const sc_logic & a, char b);
    friend const sc_logic operator ^ (const sc_logic & a, int b);
    friend const sc_logic operator ^ (sc_logic_value_t a, const sc_logic & b);
    friend const sc_logic operator ^ (bool a, const sc_logic & b);
    friend const sc_logic operator ^ (char a, const sc_logic & b);
    friend const sc_logic operator ^ (int a, const sc_logic & b);
    // relational operators and functions 
    friend bool operator == (const sc_logic & a, const sc_logic & b);
    friend bool operator == (const sc_logic & a, sc_logic_value_t b);
    friend bool operator == (const sc_logic & a, bool b);
    friend bool operator == (const sc_logic & a, char b);
    friend bool operator == (const sc_logic & a, int b);
    friend bool operator == (sc_logic_value_t a, const sc_logic & b);
    friend bool operator == (bool a, const sc_logic & b);
    friend bool operator == (char a, const sc_logic & b);
    friend bool operator == (int a, const sc_logic & b);
    friend bool operator != (const sc_logic & a, const sc_logic & b);
    friend bool operator != (const sc_logic & a, sc_logic_value_t b);
    friend bool operator != (const sc_logic & a, bool b);
    friend bool operator != (const sc_logic & a, char b);
    friend bool operator != (const sc_logic & a, int b);
    friend bool operator != (sc_logic_value_t a, const sc_logic & b);
    friend bool operator != (bool a, const sc_logic & b);
    friend bool operator != (char a, const sc_logic & b);
    friend bool operator != (int a, const sc_logic & b);
    
    // explicit conversions 
    sc_logic_value_t value() const;
    bool is_01() const;
    bool to_bool() const;
    char to_char() const;
    
    // memory (de);allocation 
    static void * operator new (size_t, void * p); // placement new 
    static void * operator new (size_t sz);
    static void operator delete (void * p, size_t sz);
    static void * operator new [] (size_t sz);
    static void operator delete [] (void * p, size_t sz);
    
    private:
    // disabled 
    explicit sc_logic (const char *);
    sc_logic & operator = (const char *);
};

extern const sc_logic SC_LOGIC_0;
extern const sc_logic SC_LOGIC_1;
extern const sc_logic SC_LOGIC_Z;
extern const sc_logic SC_LOGIC_X;

} /* end of sc_dt namespace */

#endif /* __SC_LOGIC_H__ */

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

