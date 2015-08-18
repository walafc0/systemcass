
#include "systemc.h"

#include <iostream>

#include "test.h"

using namespace std;


struct observer : sc_module {
    sc_in_clk clk;

    sc_in<int> i;

    void f() {}

    SC_HAS_PROCESS(observer);
    observer(sc_module_name) : clk ("clk"),
    i("i") {
        SC_METHOD(f);
        sensitive << clk.pos();
        dont_initialize();
    }
};


struct generator : sc_module {
    sc_in_clk clk;
    sc_out<int> o;

    void f() {
        int t = (int) (sc_time_stamp().to_double());
#ifdef SYSTEMCASS_SPECIFIC
        t = t * 1000;
#endif
        o.write(t);
    }

    SC_HAS_PROCESS(generator);
    generator(sc_module_name) : clk("clk"), o("o") {
        SC_METHOD(f);
        sensitive << clk.pos();
        dont_initialize();
    }

};



struct top_level : sc_module {
    sc_in_clk clk;

    sc_out<int> o;

    generator g;
    observer obs1, obs2;

    SC_HAS_PROCESS(top_level);
    top_level(sc_module_name) :
        clk("clk"),
        o("o"),
        g("generator"), 
        obs1("observer1"),
        obs2("observer2") {
        g.clk(clk);
        obs1.clk(clk);
        obs2.clk(clk);

        g.o(o);
        obs1.i(o);
        obs2.i(o);
    }

};


int sc_main (int argc, char ** argv) {
    sc_clock clk("top_clk", sc_time(1, sc_core::SC_NS));
    sc_signal<int> out("top_out");

    top_level t("top_level");

    t.clk(clk);
    t.o(out);

    // QM : pourquoi est-ce tout commenté ??
#if 0
    /* Open trace file */
    sc_trace_file *system_trace_file;
    system_trace_file = sc_create_vcd_trace_file ("trace_file");

    /* clks waveforms are always useful */
    sc_trace(system_trace_file, clk1, "clk1");
    sc_trace(system_trace_file, clk2, "clk2");

    /* others signals */ 
    for (int i = 0; i < 10; ++i)
        sc_trace(system_trace_file, s[i], sc_gen_unique_name ("s"));
#endif

    sc_start(sc_time(0, sc_core::SC_NS));

    /* simulation */
    int i = 0;
    while (i++ < 5) {
        sc_start(sc_time(1, sc_core::SC_NS));
        cout << out.read() << " - " << t.obs1.i.read() << " - " << t.obs2.i.read() << endl;
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

