#include <signal.h>

#include "systemc.h"
#include "test.h"


using namespace std;

struct M1_3i3o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_in<bool>  i3;
    sc_out<bool> o1;
    sc_out<bool> o2;
    sc_out<bool> o3;

    sc_signal<bool> reg;

    void eval_mealy1() {
        o1 = ~i1;
    }

    void eval_mealy2() {
        o2 = ~i2;
    }

    void tr_moore() {
        reg = i3;
    }

    void gen_moore() {
        o3 = ~reg;
    }

    SC_CTOR (M1_3i3o) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), o1("o1"),
    o2("o2"), o3("o3") {
        SC_METHOD(eval_mealy1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif

        SC_METHOD(eval_mealy2);
        sensitive << clk.neg();
        sensitive << i2;
        dont_initialize();

#ifdef SYSTEMCASS_SPECIFIC
        o2(i2);
#endif
        SC_METHOD(tr_moore);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_moore);
        sensitive << clk.neg();
        dont_initialize();
    }

};


struct M2_4i4o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_in<bool>  i3;
    sc_in<bool>  i4;
    sc_out<bool> o1;
    sc_out<bool> o2;
    sc_out<bool> o3;
    sc_out<bool> o4;
    sc_signal<sc_uint<2> > reg;

    void tr_moore() {
        reg = ((i1.read() & i3.read()) ? 2 : 0) + (i2.read() | i4.read());
    }

    void gen_moore() {
        o1 = reg.read() & 1;
        o2 = reg.read() & 2;
        o3 = ~(reg.read() & 1);
        o4 = ~(reg.read() & 2);
    }


    SC_CTOR (M2_4i4o) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), i4("i4"), o1("o1"), o2("o2"), o3("o3"), o4("o4")  {
        SC_METHOD(tr_moore);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_moore);                     
        sensitive << clk.neg();                   
        dont_initialize();
    }                                           

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock");
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


    M1_3i3o a("a");
    M1_3i3o b("b");
    M2_4i4o Moore("Moore");

    a.clk(signal_clk);
    b.clk(signal_clk);
    Moore.clk(signal_clk);

    Moore.o1(s1);
    a.i1(s1);

    a.o1(s2);
    b.i1(s2);

    b.o1(s3);
    Moore.i1(s3);

    Moore.o2(s4);
    b.i2(s4);

    b.o2(s5);
    a.i2(s5);

    a.o2(s6);
    Moore.i2(s6);

    Moore.o3(s7);
    b.i3(s7);

    Moore.o4(s8);
    a.i3(s8);

    b.o3(s9);
    Moore.i4(s9);

    a.o3(s10);
    Moore.i3(s10);

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

