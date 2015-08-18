/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_numrep.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   04_05_2005                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_NUMREP_H__
#define __SC_NUMREP_H__

#include <cstdio>
#include <iostream>

#include "sc_string.h"

namespace sc_dt {

typedef enum {
    SC_NOBASE = 0,
    SC_BIN = 2,   // binary
    SC_OCT = 8,   // octal, two's complement
    SC_DEC = 10,  // decimal
    SC_HEX = 16,  // hexadecimal, two's complement
    SC_BIN_US,    // binary, unsigned
    SC_BIN_SM,    // binary, sign magnitude
    SC_OCT_US, 
    SC_OCT_SM,
    SC_HEX_US,
    SC_HEX_SM,
    SC_CSD        // canonical signed digit
} sc_numrep;


inline const unsigned int bitstring_to_uint(const char * val) { 
    unsigned int i, res = 0;
    for (i = 0; val[i] != '\0'; ++i) {
        res <<= 1;
        res |= (val[i] == '1') ? 1 : 0;
    }
    return res;    
}


template <typename T>
const sc_string to_string(T val, size_t nbits, sc_numrep n) { 
    sc_string s(nbits + 3, '0');
    switch (n) {
        case SC_HEX :
        {
            char t[64];
            char format[64];
            sprintf(format, "0x0%%0%dx", nbits / 4);
            sprintf(t, format, val);
            s = t;
            break;
        }
        case SC_BIN :
        {
            int i;
            s[1] = 'b';
            for (i = nbits - 1; i >= 0; --i)
            {
                s[i + 3] = (val & 1)?'1':'0';
                val >>= 1;
            } 
            break;
        }
        default:
        {
                std::cerr << "error\n"; return s;
        }
    }
    return s;
}

const std::string to_string(sc_numrep);

} // end of sc_dt namespace

using sc_dt::sc_numrep;
using sc_dt::SC_BIN;
using sc_dt::SC_HEX;
using sc_dt::SC_OCT; 
using sc_dt::SC_DEC;
using sc_dt::SC_HEX;
using sc_dt::SC_BIN_US;
using sc_dt::SC_BIN_SM;
using sc_dt::SC_OCT_US;
using sc_dt::SC_OCT_SM;
using sc_dt::SC_HEX_US;
using sc_dt::SC_HEX_SM;
using sc_dt::SC_CSD;

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

