
#include <iostream>
#include <string>

#include "systemc.h"
#include "test.h"


using namespace std;


struct internal_model : sc_module {
    sc_in<int> i;
    sc_out<int> o;
    internal_model (sc_module_name n) : sc_module (n), i("i"), o("o") {}
};


struct model : sc_module {
    sc_in<int> i1, i2, i3;
    sc_out<int> o1, o2, o3;
    sc_signal<int> r1, r2;
    internal_model internal;
    model(sc_module_name n) : sc_module (n),
    i1("i1"), i2("i2"), i3("i3"),
    o1("o1"), o2("o2"), o3("o3"),
    r1("r1"), r2("r2"),
    internal("internal") {
        internal.o (o3);
        internal.i (i3);
    }
};


int sc_main (int argc, char ** argv) {
    model m("m");
    sc_clock clk("clock");
    sc_signal<int> s1("s1"), s2("s2"), s3("s3"), s4("s4");

    m.i1(s1);
    m.i3(s1);
    m.o1(s4);
    m.o2(s2);
    m.o3(s3);

    sc_start(sc_time(0, sc_core::SC_NS));

    sc_start(sc_time(1, sc_core::SC_NS));
    sc_start(sc_time(10, sc_core::SC_NS));

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

