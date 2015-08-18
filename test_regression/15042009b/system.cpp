
#include <iostream>

#include "systemc.h"
#include "test.h"



using namespace std;

typedef sc_uint<36> data_type; 

int sc_main (int argc, char ** argv) {
    sc_uint<36> a, b, c;
    const long long int ca = 0xf00000000LLU;
    const long long int cb = 0x123456789LLU;
    a = ca;
    b = cb;

    c = a & b;
    if (c != (ca & cb)) {
        return 1;
    }

    c = a;
    if (a != c) {
        return 1;
    }
    if (!(a == c)) {
        return 1;
    }

    cout << "OK" << endl;
    return 0;
}


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

