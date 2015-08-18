
#include <iostream>

#include "systemc.h"
#include "test.h"

using namespace std;


#define MASK 0x0000000FFFFFFFFFLLU


typedef sc_uint<36> data_type; 

struct observer : sc_module {
    sc_in_clk clk;

    sc_in<data_type> i;

    void f() {
        printf("0x%016llx\n", (unsigned long long int) i.read());
        data_type t = i.read();

        if (t == 7) {
            cout << "(== 0x7)\n";
        }
        t <<= 1;

        if (t == 14) {
            cout << "((t << 1) == 0xe)\n";
        }

        t += 1;

        if (t == 15) {
            cout << "(((t << 1) + 1) == 0xf)\n";
        }

        t = t >> 1;

        if (t == 7) {
            cout << "((((t << 1) + 1) >> 1) == 0x7)\n";
        }

    }

    SC_HAS_PROCESS(observer);
    observer(sc_module_name) : clk ("clk"), i("i") {
        SC_METHOD(f);
        sensitive << clk.pos();
        dont_initialize();
    }

};


struct generator : sc_module {
    sc_in_clk clk;

    sc_out<data_type> o;
    sc_in<data_type>  i;

    long long int t;

    void f() {
        t <<= 1;
        t++;
        o.write(t);
    }

    SC_HAS_PROCESS(generator);
    generator(sc_module_name) : clk ("clk"), o("o") {
        t = 1;
        SC_METHOD(f);
        sensitive << clk.neg() << i;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o(i);
#endif
    }

};

struct top_level : sc_module {
    sc_in_clk clk;

    sc_out<data_type> o;
    sc_in <data_type> i;

    generator g;
    observer obs1, obs2;

    SC_HAS_PROCESS(top_level);
    top_level(sc_module_name) :
    clk ("clk"),
    o("o"),
    i("i"),
    g("generator"), 
    obs1("observer1"),
    obs2("observer2") {
        g.clk(clk);
        obs1.clk(clk);
        obs2.clk(clk);

        g.i(i);
        g.o(o);
        obs1.i(o);
        obs2.i(o);

#ifdef SYSTEMCASS_SPECIFIC
        o(i); // wrong declaration that needs detection (no sc_method)
#endif
    }

};


int sc_main(int argc, char ** argv) {
    sc_clock             clk("top_clk");
    sc_signal<data_type> out("top_out");
    sc_signal<data_type> in("top_in");

    top_level t("top_level");

    t.clk(clk);
    t.o(out);
    t.i(in);

    /* initilization */
    sc_start(sc_time(0, sc_core::SC_NS));

    /* simulation */
    int i = 0;
    while (i++ < 40) {
        sc_start(sc_time(1, sc_core::SC_NS));
        ASSERT(out.read() == t.obs1.i.read())
        ASSERT(out.read() == t.obs2.i.read())
        ASSERT(out.read() == (t.g.t & MASK))
        printf("0x%08x - 0x%08x - 0x%08x - 0x%llx\n", (unsigned int) out.read(), (unsigned int) t.obs1.i.read(), (unsigned int) t.obs2.i.read(), (t.g.t & MASK));
    }

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

