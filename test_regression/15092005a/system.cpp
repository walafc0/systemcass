
#include <iostream>

#include "systemc.h"
#include "test.h"


using namespace std;

struct observer : sc_module {
    sc_in_clk clk;

    sc_in<int> i;

    void f() {}

    SC_HAS_PROCESS(observer);
    observer(sc_module_name) : clk("clk"), i("i") {
        SC_METHOD(f);
        sensitive << clk.pos();
        dont_initialize();
    }
};


struct generator : sc_module {
    sc_in_clk clk;

    sc_out<int> o;
    sc_in<int> i;

    void f() {
        int t = (int) (sc_time_stamp().to_double());
        o.write(t);
    }

    SC_HAS_PROCESS(generator);
    generator(sc_module_name) : clk("clk"), o("o") {
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

    sc_out<int> o;
    sc_in <int> i;

    generator g;
    observer obs1, obs2;

    SC_HAS_PROCESS(top_level);
    top_level(sc_module_name) :
    clk("clk"),
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


int sc_main (int argc, char ** argv) {
    sc_clock clk("top_clk");
    sc_signal<int> out("top_out");
    sc_signal<int> in ("top_in");

    top_level t("top_level");

    t.clk(clk);
    t.o(out);
    t.i(in);

    sc_start(sc_time(0, sc_core::SC_NS));

    int i = 0;
    while (i++ < 5) {
        sc_start(sc_time(1, sc_core::SC_NS));
        ASSERT(out.read() == t.obs1.i.read());
        ASSERT(out.read() == t.obs2.i.read());
    }

    cout << "OK" << endl;
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

