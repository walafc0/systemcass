// Le test échoue 
// Par ailleurs, la référence est la trace de systemcass ; je pense qu'il faudrait changer ça

#include "systemc.h"

#include "test.h"


using namespace std;

struct hard : sc_module {
    sc_in_clk      clk;
    sc_in<bool>    resetn;
    sc_in <int>    i1;
    sc_out<int>    o1;
    sc_signal<int> r1;

    void fg() {
        o1 = r1;
    }

    void ft() {
        if (resetn.read()) {
            r1 = i1;
        }
        else {
            r1 = 0x0;
        }
    }

    SC_HAS_PROCESS(hard);

    hard(sc_module_name) {
        SC_METHOD(ft);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(fg);
        sensitive << clk.neg();
        dont_initialize();
    }
};


int sc_main (int argc, char ** argv) {
    sc_clock clk1("clk1", 1, 0.5, 0, true);
    sc_signal<bool> resetn;
    sc_signal<int> s[10];
    hard a("a");

    a.clk(clk1);
    a.resetn(resetn);

    a.i1(s[0]);
    a.o1(s[1]);

    /* Open trace file */
    sc_trace_file * system_trace_file;
    system_trace_file = sc_create_vcd_trace_file("trace_file");

    /* clks waveforms are always useful */
    sc_trace(system_trace_file, resetn, "resetn");
    sc_trace(system_trace_file, s[0], "i1");
    sc_trace(system_trace_file, s[1], "o1");
    sc_trace(system_trace_file, clk1, "clk1");

    /* initilization */
    sc_start(sc_time(0, sc_core::SC_NS));

    s[0] = 0;
    resetn = 0;

    sc_start(sc_time(5, sc_core::SC_NS));

    s[0] = 0x7B;
    resetn = 1;

    sc_start(sc_time(1, sc_core::SC_NS));

    for (int i = 0; i < 10; i++) {
        s[0] = i;
        sc_start(sc_time(1, sc_core::SC_NS));
        ASSERT(s[1].read() == i);
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

