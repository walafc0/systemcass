
#include <iostream>

#include "systemc.h"
#include "test.h"

using namespace std;


int sc_main (int argc, char ** argv) {

    sc_uint<32> a;
    sc_uint<3>  b;
    sc_uint<2>  c;
    sc_uint<5>  d;
    sc_uint<8>  e;
    sc_uint<10> f;

    a = 0xCFFFFFFF;
    b = 0x7;
    c = 0x3;
    d = 0xA0;
    e = 0x11;
    f = 0x1A0;

    cout << "a = 0x" << hex << (unsigned int) a << " = " << a.to_string(SC_BIN) << "\n";
    ASSERT(a.to_string (SC_BIN) == "0b011001111111111111111111111111111");

    cout << "b = 0x" << hex << (unsigned int) b << " = " << b.to_string(SC_BIN) << "\n";
    ASSERT(b.to_string (SC_BIN) == "0b0111");

    cout << "c = 0x" << hex << (unsigned int) c << " = " << c.to_string(SC_BIN) << "\n";
    ASSERT(c.to_string (SC_BIN) == "0b011");

    cout << "d = 0x" << hex << (unsigned int) d << " = " << d.to_string(SC_BIN) << "\n";
    ASSERT(d.to_string (SC_BIN) == "0b000000");

    cout << "e = 0x" << hex << (unsigned int) e << " = " << e.to_string(SC_BIN) << "\n";
    ASSERT(e.to_string (SC_BIN) == "0b000010001");

    cout << "f = 0x" << hex << (unsigned int) f << " = " << f.to_string(SC_BIN) << "\n";
    ASSERT(f.to_string (SC_BIN) == "0b00110100000");

    cout << "b,e = 0x" << hex << (unsigned int) (b,e) << " = " << (b, e).to_string(SC_BIN) << "\n";
    ASSERT((b, e).to_string (SC_BIN) == "0b011100010001");

    cout << "b,c,d,e,f = 0x" << hex << (unsigned int) (b, c, d, e, f) << " = " << (b, c, d, e, f).to_string (SC_BIN) << "\n";
    ASSERT((b, c, d, e, f).to_string (SC_BIN) == "0b01111100000000100010110100000");

    a = b,c,d,e,f;
    cout << "a = b,c,d,e,f; a = 0x" << hex << (unsigned int) a << " = " << a.to_string (SC_BIN) << "\n";
    ASSERT(a.to_string (SC_BIN) == "0b000000000000000000000000000000111");

    sc_uint<3> tb = b.range (2, 0);
    cout << "b.range (2,0) = 0x" << hex << (unsigned int) tb << " = " << tb.to_string (SC_BIN) << "\n";
    ASSERT(tb.to_string (SC_BIN) == "0b0111");

    sc_uint<5> te = e.range(5, 1);
    cout << "e.range (5,1) = 0x" << hex << (unsigned int) te << " = " << te.to_string(SC_BIN) << "\n";
    ASSERT(te.to_string(SC_BIN) == "0b001000");

    sc_uint<5> tf = f.range(9, 5);
    cout << "f.range (9,5) = 0x" << hex << (unsigned int) tf << " = " << tf.to_string (SC_BIN) << "\n";
    ASSERT(tf.to_string (SC_BIN) == "0b001101");

    sc_uint<5> tf2 = f.range(8, 4);
    a = (tf2, b, c, d, e, f);
    cout << "a = f.range (7,3),b,c,d,e,f; a = 0x" << hex << (unsigned int) a << " = " << a.to_string (SC_BIN) << "\n";
    ASSERT(a.to_string (SC_BIN) == "0b010101111100000000100010110100000");

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

