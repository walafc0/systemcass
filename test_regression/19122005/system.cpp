
#include "systemc.h"
#include "test.h"



using namespace std;


struct test : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<int>   i2;
    sc_in<int>   i3;
    sc_out<bool> o1;
    sc_out<int>  o2;
    sc_out<int>  o3;
    sc_inout< sc_uint<32> > io4;
    sc_out<int>  mealy1;
    sc_out<int>  mealy2;

    sc_signal<bool> reg1;
    sc_signal<int>  reg2;
    sc_signal<int>  reg3;
    sc_signal<sc_uint<32> > reg9;
    sc_signal<int>  reg10;

    void fmealy1() {
        mealy1 = i2.read();
    }

    void fmealy2() {
        mealy2 = i2.read() + reg2.read();
    }

    void gen() {
        o1  = reg1.read() ^ true;
        o2  = reg2.read() + 1;
        o3  = reg3.read() + 1;
        io4 = reg9.read() + 1;
    }

    void trans() {
        reg1 = reg1.read() ^ 1;
        reg2 = reg2.read() + 1;
        reg3 = reg3.read() + 1;
        reg9 = reg9.read() + 2;
        reg10 = reg10.read() + i2.read();
    }

    SC_HAS_PROCESS(test);
    test(sc_module_name n) : sc_module(n),
    clk("clk"),
    i1("i1"), i2("i2"), i3("i3"),
    o1("o1"), o2("o2"), o3("o3"),
    io4("io4"), 
    mealy1("mealy1_equivalent_to_i2"),
    mealy2("mealy2_equivalent_to_i2_plus_reg2"),
    reg1("reg1_cycle_number_not_parity"),
    reg2("reg2_cycle_number"),
    reg3("reg3_cycle_number"),
    reg9("reg9_cycle_number_x2"),
    reg10("reg10_sum_cycle_number") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen);
        sensitive << clk.neg();
        dont_initialize();

        SC_METHOD(fmealy1);
        sensitive << i2;
        dont_initialize();

        SC_METHOD(fmealy2);
        sensitive << i2 << clk.neg();
        dont_initialize();
    }
};


int usage (const char * com) {
    cout << "Usage :\n" << com << " [#cycles]\n";
    return EXIT_FAILURE;
}


sc_signal<bool> s01("i1_cycle_number_not_parity"), s02("o2_not_reg1");
sc_signal<int>  s03("i2_cycle_number_x2"), s04("o2_reg2_plus_one");
sc_signal<int>  s05("i3_cycle_number_x3"), s06("o3_reg3_plus_one");
sc_signal< sc_uint<32> > s16("io4_reg9_plus_one");
sc_signal<int>  s17("mealy1_equivalent_to_i2");
sc_signal<int>  s18("mealy2_i2_plus_reg2");


int s(int i) {
    int val = 0;
    while (i) {
        val += i--;
    }
    return val;
}


void * func() {
    cerr << "func () at #" << sc_time_stamp() << endl;
    int i = (int) (sc_time_stamp().to_double()) / 1000;
    s01  = (i & 1) > 0;
    s03  = i * 2;
    s05  = i * 3;
    ++i;
    return 0;
}


void save() {
#ifdef SYSTEMCASS_SPECIFIC
    char name[256];
    sprintf(name, "test_systemcass_%d.dat", ((int) sc_time_stamp().to_double() / 1000));
    sc_save_simulation(name);
#endif
}


int sc_main (int argc, char * argv[]) {
    sc_clock  signal_clk("my_clock", 1, 0.5);

    test test1("test1");
    test1.clk(signal_clk);
    test1.i1(s01);
    test1.o1(s02);
    test1.i2(s03);
    test1.o2(s04);
    test1.i3(s05);
    test1.o3(s06);
    test1.io4(s16);
    test1.mealy1(s17);
    test1.mealy2(s18);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

#ifndef SOCVIEW
    if (argc != 2) {
        return usage(argv[0]);
    }

    int nb = atoi(argv[1]);

    if (nb == 0) {
        return usage(argv[0]);
    } 

    int i = 0;
    save();
    while (i++ < nb) {
        func();
        sc_start(sc_time(1, sc_core::SC_NS));
        save();
    }
#else
    debug(&func);
#endif

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

