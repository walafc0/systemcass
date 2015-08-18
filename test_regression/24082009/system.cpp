
#include <signal.h>

#include "systemc.h"
#include "test.h"

using namespace std;

struct A : sc_module {
    sc_in_clk clk;
    sc_in <int> i1;
    sc_signal <int> r1;
    sc_out <int> o1;

    void transition() {
        int i;
        for (i = 0; i < 1000; ++i);
        r1 = i1;
    }

    void gen_moore() {
        int i;
        for (i = 0; i < 1000; ++i);
        o1 = r1;
    }

    SC_CTOR (A) : clk("clk"), o1("o1") {
        SC_METHOD(transition);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_moore);
        sensitive << clk.neg();
        dont_initialize();
    }

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock");
    sc_signal<int> s1("s1"), s2("s2"), s3("s3"), s4("s4");

    A a("a");
    A b("b");

    a.clk(signal_clk);
    b.clk(signal_clk);

    a.i1(s1);
    a.o1(s2);

    b.i1(s3);
    b.o1(s4);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

    s1.write(1);

    sc_start(sc_time(100000, sc_core::SC_NS));

    cout << s1.read() << endl;
    cout << s2.read() << endl;

    return EXIT_SUCCESS;
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

