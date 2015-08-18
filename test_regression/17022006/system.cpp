
#include <cstring>

#include "systemc.h"
#include "test.h"


using namespace std;


struct test : sc_module {
    sc_in_clk clk;
    sc_in<int> i1;
    sc_in<bool> i2[4];
    sc_out<int> o1;
    sc_out<char> o2[3];

    sc_signal<bool> reg1;
    sc_signal<int> reg2;
    sc_signal<int> reg3;
    sc_signal<int> reg4[10];

    void trans() {}

    void gen() {}

    SC_HAS_PROCESS(test);

    test(sc_module_name n) : sc_module(n),
    clk("clk"),
    i1("i1"), 
    o1("o1"),
    reg1("reg1"),
    reg2("reg2"),
    reg3("reg3") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen);
        sensitive << clk.neg();
        dont_initialize();

#ifdef NONAME_RENAME
        char str[100];
        for (int i = 0; i < 3; i++) {
            sprintf(str, "o2_%d", i);
            o2[i].rename(str);
        }
        for (int i = 0; i < 4; i++) {
            sprintf(str, "i2_%d", i);
            i2[i].rename(str);
        }
        for (int i = 0; i < 10; i++) {
            sprintf(str, "reg4_%d", i);
            reg4[i].rename(str);
        }
#endif
    }

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock");
    sc_signal<int> s01("s01");
    sc_signal<bool> s02_0("s02_0"), s02_1("s02_1"), s02_2("s02_2"), s02_3("s02_3");
    sc_signal<int> s03("s03");
    sc_signal<char> s04_0("s04_0"), s04_1("s04_1"), s04_2("s04_2");

    test test1("test1");
    test1.clk(signal_clk);
    test1.i1(s01);
    test1.i2[0](s02_0);
    test1.i2[1](s02_1);
    test1.i2[2](s02_2);
    test1.i2[3](s02_3);
    test1.o1(s03);
    test1.o2[0](s04_0);
    test1.o2[1](s04_1);
    test1.o2[2](s04_2);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

    ASSERT(strcmp(test1.o1.name(), "test1.o1") == 0);
    ASSERT(strcmp(test1.i1.name(), "test1.i1") == 0);
    ASSERT(strcmp(test1.reg1.name(), "test1.reg1") == 0);
    ASSERT(strcmp(test1.reg2.basename(), "reg2") == 0);
    ASSERT(strcmp(test1.reg3.name(), "test1.reg3") == 0);

#if defined(SYSTEMCASS_SPECIFIC)
    ASSERT(strcmp(test1.o2[1].name(), "test1.o2_1") == 0);
    ASSERT(strcmp(test1.i2[2].basename(), "i2_2") == 0);
    ASSERT(strcmp(test1.reg4[1].name(), "test1.reg4_1") == 0);
    ASSERT(strcmp(test1.reg4[9].basename(), "reg4_9") == 0);
#endif

    cout << "OK" << endl;

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

