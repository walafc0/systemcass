

#include <iostream>

#include "systemc.h"
#include "test.h"


using namespace std;


int sc_main (int argc, char ** argv) {

    sc_uint<17> a;
    sc_uint<3>  b;
    sc_uint<2>  c1;
    sc_uint<2>  c2;
    sc_uint<5>  d;
    sc_uint<7>  e;
    sc_uint<1>  f;
    sc_uint<16> g;
    sc_uint<8>  h;
    sc_uint<32> i;
    sc_uint<17> j;

    b  = 0x7;
    c1 = 0x3;
    c2 = 0x1;
    d  = 0xA;
    e  = 0x11;
    g  = 0xabcd;

    // Concatenation
    a = (b, (c1 | c2),  d & (b, c1), e);
    j = (b, c1, d , e);

    f = b.range (0, 0);
    h = g.range (11, 4);
    i = e;

    cout << "a = 0x" << hex << (unsigned int) a << " = " << a.to_string(SC_BIN) << "\n";
    //ASSERT(a.to_string(SC_BIN) == "0b000000111100010001");

    cout << "f = 0x" << hex << (unsigned int) f << " = " << f.to_string(SC_BIN) << "\n";
    //ASSERT(f.to_string(SC_BIN) == "0b01");

    cout << "h = 0x" << hex << (unsigned int) h << " = " << h.to_string(SC_BIN) << "\n";
    //ASSERT(h.to_string(SC_BIN) == "0b010111100");

    cout << "i = 0x" << hex << (unsigned int) i << " = " << i.to_string(SC_BIN) << "\n";
    cout << "j = 0x" << hex << (unsigned int) j << " = " << j.to_string(SC_BIN) << "\n";

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

