
#include <stdint.h>

#include "systemc.h"
#include "test.h"

using namespace std;


struct test : sc_module {
    sc_in_clk               clk;
    sc_in<bool>             i1;
    sc_in<char>             i2;
    sc_in<int>              i3;
    sc_in<sc_int<4> >       i4;
    sc_in<sc_uint<4> >      i5;
    sc_in<sc_uint<64> >     i6;
    sc_out<bool>            o1;
    sc_out<char>            o2;
    sc_out<int >            o3;
    sc_out<sc_int<4> >      o4;
    sc_out<sc_uint<4> >     o5;
    sc_out<sc_uint<64> >    o6;
    sc_inout<sc_uint<64> >  io1;
    sc_inout<sc_uint<32> >  io4;

    sc_signal<bool>         reg1;
    sc_signal<char>         reg2;
    sc_signal<int>          reg3;
    sc_signal<sc_int<4> >   reg4;
    sc_signal<sc_uint<4> >  reg5;
    sc_signal<sc_uint<64> > reg6;
    sc_signal<sc_uint<32> > reg9;

    void gen() {
        o1  = reg1.read() ^ true;
        o2  = reg2.read() + 1;
        o3  = reg3.read() + 1;
        o4  = reg4.read() + 1;
        o5  = reg5.read() + 1;
        o6  = reg6.read() + 1;
        io1 = reg6.read() * 2 + 1;
        io4 = reg9.read() + 1;
    }

    void trans() {
        reg1 = reg1.read() ^ 1;
        reg2 = reg2.read() + 1;
        reg3 = reg3.read() + 1;
        reg4 = reg4.read() + 1;
        reg5 = reg5.read() + 1;
        reg6 = reg6.read() * 2 + 1;
        reg9 = reg9.read() + 1;
    }

    SC_CTOR (test) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), o1("o1"),
    o2("o2"), o3("o3"), io1("io1"), io4("io4"), reg9("reg9") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen);
        sensitive << clk.neg();
        dont_initialize();
    }

};



int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock", 1, 0.5);
    sc_signal<bool> s1 ("s01"), s2 ("s02");
    sc_signal<char> s3 ("s03"), s4 ("s04");
    sc_signal<int>  s5 ("s05"), s6 ("s06");
    sc_signal< sc_int<4> >   s7 ("s07"), s8 ("s08");
    sc_signal< sc_uint<4> >  s9 ("s09"), s10("s10");
    sc_signal< sc_uint<64> > s11("s11"), s12("s12"), s13("s13");
    sc_signal< sc_uint<32> > s16("s16");

    test test1("test1");
    test1.clk(signal_clk);
    test1.i1(s1);
    test1.o1(s2);
    test1.i2(s3);
    test1.o2(s4);
    test1.i3(s5);
    test1.o3(s6);
    test1.i4(s7);
    test1.o4(s8);
    test1.i5(s9);
    test1.o5(s10);
    test1.i6(s11);
    test1.o6(s12);
    test1.io1(s13);
    test1.io4(s16);

    /* Open trace file */
    sc_trace_file * system_trace_file;
    system_trace_file = sc_create_vcd_trace_file("trace_file");
    /* clk waveform is always useful */
    sc_trace(system_trace_file, signal_clk, "clk");

    bool b1 = 0;
    uint64_t l1 = 0;

#define TRACE(x) sc_trace (system_trace_file, x, #x);
    TRACE(b1);
    TRACE(l1);
    TRACE(test1.i1);
    TRACE(test1.i2);
    TRACE(test1.i3);
    TRACE(test1.i4);
    TRACE(test1.i5);
    TRACE(test1.i6);
    TRACE(test1.o1);
    TRACE(test1.o2);
    TRACE(test1.o3);
    TRACE(test1.o4);
    TRACE(test1.o5);
    TRACE(test1.o6);
    TRACE(test1.io1);
    TRACE(test1.io4);
    TRACE(s1);
    TRACE(s2);
    TRACE(s3);
    TRACE(s4);
    TRACE(s5);
    TRACE(s6);
    TRACE(s7);
    TRACE(s8);
    TRACE(s9);
    TRACE(s10);
    TRACE(s11);
    TRACE(s12);
    TRACE(s13);
    TRACE(s16);
#undef TRACE

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

    ASSERT(test1.i1.read() == false);
    ASSERT(s4.read() == 0);
    ASSERT(s10.read() == 0);
    ASSERT(s16.read() == 0);

    sc_start(sc_time(1, sc_core::SC_NS));

    b1 = 1;
    l1 = 5;

    int i;
    for (i = 1; i < 60; ++i) {
        l1 += i;
        b1 ^= true;
        s1  = s1.read() ^ true;
        s3  = s3.read() + 2;
        s5  = s5.read() + 3;
        s7  = s7.read() + 4;
        s9  = s9.read() + 5;
        s11 = (s11.read() << 2) + 1;
        sc_start(sc_time(1, sc_core::SC_NS));
    }

    sc_close_vcd_trace_file(system_trace_file);

    return EXIT_SUCCESS;
}

#undef sc_inout

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

