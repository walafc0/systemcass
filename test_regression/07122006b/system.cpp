
#include <systemc.h>
#include <signal.h>
#include <iostream>
#include <fstream>

#include "test.h"

using namespace std;

struct inner_test : public sc_module {
    virtual void transition() {
        cout << "This function is a virtual one.\n";
    }

    inner_test(sc_module_name n) {}
};


struct test : public inner_test {
    sc_in<bool> clk;
    sc_in<bool> resetn;
    sc_out<int> o;
    sc_in<int>  i;
    sc_signal<int> reg;

    virtual void transition();
    void gen_moore();
    void gen_mealy();

    SC_HAS_PROCESS(test);
    test(sc_module_name n) : inner_test(n) {
        SC_METHOD(transition);
        sensitive << clk.pos();
        dont_initialize();
    
        SC_METHOD(gen_moore);
        sensitive << clk.neg();
        dont_initialize();
        
        SC_METHOD(gen_mealy);
        sensitive << clk.neg() << i;
        dont_initialize();
    }
};

void test::transition() {
    std::cout << "transition\n";
}

void test::gen_moore() {
    std::cout << "gen_moore\n";
}

void test::gen_mealy() {
    std::cout << "gen_mealy\n";
}



int sc_main(int argc, char * argv[]) {
    sc_clock clk("clk");
    sc_signal<bool> resetn("resetn");
    sc_signal<int> in ("in");
    sc_signal<int> out("out");

    test test("test");
    test.clk(clk);
    test.resetn(resetn);

    test.i(in);
    test.o(out);

    sc_start(sc_time(0, sc_core::SC_NS));

    resetn = false;
    sc_start(sc_time(3, sc_core::SC_NS));
    resetn = true;
    sc_start(sc_time(10, sc_core::SC_NS));

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

