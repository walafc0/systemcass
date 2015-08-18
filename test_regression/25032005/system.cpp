#include <signal.h>

#include "systemc.h"
#include "test.h"


using namespace std;

struct A : sc_module {
    sc_in_clk clk;
    sc_out<bool> o1;

    void eval() {
        o1 = (rand() % 2) == 0;
    }

    SC_CTOR (A) : clk("clk"), o1("o1") {
        SC_METHOD(eval);
        sensitive << clk.neg();
        dont_initialize();
    }
};


struct B : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_out<bool> o1;
    sc_in<bool>  i2;
    sc_out<bool> o2;

    void eval1() {
        o1 = ~i1;
    }

    void eval2() {
        o2 = ~i2;
    }

    SC_CTOR (B) : clk("clk"), i1("i1"), o1("o1"), i2("i2"), o2("o2") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
        SC_METHOD(eval2);
        sensitive << clk.neg();
        sensitive << i2;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o2(i2);
#endif
    }

};


struct C : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_out<bool> o1;
    sc_in<bool>  i2;

    void eval1() {
        o1 = i1 ^ i2;
    }

    SC_CTOR (C) : clk ("clk"), i1("i1"), o1("o1"), i2("i2") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1 << i2;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
        o1(i2);
#endif
    }

};


struct D : sc_module {
    sc_in_clk clk;
    sc_out<bool> o1;

    void eval() {
        o1 = (rand() % 2) == 0;
    }

    SC_CTOR (D) : clk("clk"), o1("o1") {
        SC_METHOD(eval);
        sensitive << clk.neg();
        dont_initialize();
    }

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock");
    sc_signal<bool> s1("s1"), s2("s2"), s3("s3"), s4("s4"), s5("s5");

    A a("a");
    B b("b");
    C c("c");
    D d("d");

    a.clk(signal_clk);
    b.clk(signal_clk);
    c.clk(signal_clk);
    d.clk(signal_clk);

    a.o1(s1);
    b.i1(s1);

    d.o1(s2);
    c.i2(s2);

    b.o1(s3);
    c.i1(s3);

    c.o1(s4);
    b.i2(s4);

    b.o2(s5);

    // Init & run
    sc_start(sc_time(1, sc_core::SC_NS));

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

