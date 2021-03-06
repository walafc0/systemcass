
#include <signal.h>

#include "systemc.h"
#include "test.h"


using namespace std;


struct M1_1i1o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_out<bool> o1;

    void eval_mealy1() {
        o1 = ~i1;
    }

    SC_CTOR (M1_1i1o) : clk("clk"), i1("i1"), o1("o1") {
        SC_METHOD(eval_mealy1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
    }

};


struct M1_3i2o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_in<bool>  i3;
    sc_out<bool> o1;
    sc_out<bool> o2;

    void eval_mealy1() {
        o1 = ~i1 & i2;
        o2 = ~i2 & i3;
    }


    SC_CTOR (M1_3i2o) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), o1("o1"), o2("o2") {
        SC_METHOD(eval_mealy1);
        sensitive << clk.neg();
        sensitive << i1 << i2;
        sensitive << i3;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
        o1(i2);
        o2(i2);
        o2(i3);
#endif
    }

};


struct M2_1i1o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_out<bool> o1;

    sc_signal<bool> reg;

    void tr_moore1() {
        reg = ~i1;
    }

    void gen_moore1() {
        o1 = ~reg;
    };

    SC_CTOR (M2_1i1o) : clk("clk"), i1("i1"), o1("o1") {
        SC_METHOD(tr_moore1);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_moore1);
        sensitive << clk.neg();
        dont_initialize();
    }

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock",1, 0.5);
    sc_signal<bool> s1 ("s01"),
        s2 ("s02"),
        s3 ("s03"),
        s4 ("s04"),
        s5 ("s05"),
        s6 ("s06"),
        s7 ("s07"),
        s8 ("s08"),
        s9 ("s09"),
        s10("s10"),
        s11("s11"),
        s12("s12"),
        s13("s13"),
        s14("s14"),
        s15("s15");


    M1_3i2o a("a");
    M1_1i1o b("b");
    M1_3i2o c("c");
    M1_1i1o d("d");
    M1_1i1o e("e");

    a.clk(signal_clk);
    b.clk(signal_clk);
    c.clk(signal_clk);
    d.clk(signal_clk);
    e.clk(signal_clk);

    a.i1(s1);

    a.o1(s2);
    b.i1(s2);

    b.o1(s3);
    c.i1(s3);

    c.o1(s4);

    a.i2(s5);

    a.o2(s6);
    c.i2(s6);
    d.i1(s6);

    c.o2(s7);

    a.i3(s8);

    d.o1(s9);
    e.i1(s9);

    e.o1(s10);
    c.i3(s10);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

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

