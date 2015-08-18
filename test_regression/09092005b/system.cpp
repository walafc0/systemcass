
#include "systemc.h"

#include "test.h"

using namespace std;

struct hard : sc_module {
    sc_in_clk clk;
    sc_in <int> i1, i2;
    sc_out<int> o1, o2;

    void f() {
        o1 = i1;
        o2 = i2;
    }

    SC_HAS_PROCESS(hard);

    hard(sc_module_name) {
        SC_METHOD(f);
        sensitive << i1 << i2;
        dont_initialize();

#ifdef SYSTEMCASS_SPECIFIC
        o1 (i1);
        o2 (i2);
#endif
    }

};


int sc_main (int argc, char ** argv) {
    sc_clock clk1("clk1");
    sc_clock clk2("clk2", 1, 0.5, 0, false);
    sc_signal<int> s[10];
    hard a("a");
    hard b("b");
    hard c("c");

    a.clk(clk1);
    b.clk(clk2);
    c.clk(clk2);

    a.i1(s[0]);

    a.o1(s[1]);
    b.i1(s[1]);

    b.o1(s[2]);

    b.i2(s[3]);

    b.o2(s[4]);
    a.i2(s[4]);

    a.o2(s[5]);

    c.i1(s[6]);
    c.o1(s[7]);
    c.i2(s[8]);
    c.o2(s[9]);

    /* Open trace file */
    sc_trace_file *system_trace_file;
    system_trace_file = sc_create_vcd_trace_file("trace_file");

    /* clks waveforms are always useful */
    sc_trace(system_trace_file, clk1, "clk1");
    sc_trace(system_trace_file, clk2, "clk2");

    /* others signals */ 
    for (int i = 0; i < 10; ++i) {
        sc_trace(system_trace_file, s[i], sc_gen_unique_name ("s"));
    }

    /* initilization */
    sc_start(sc_time(0, sc_core::SC_NS));

    s[0] = 1;
    s[3] = 1;

    sc_start(sc_time(1, sc_core::SC_NS));

    s[0] = 123;
    s[3] = 321;

    sc_start(sc_time(1, sc_core::SC_NS));

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

