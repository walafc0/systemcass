

#include <signal.h>
#include <iostream>
#include <fstream>

#include "systemc.h"
#include "test.h"

#include "struct_test.h"


using namespace std;


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

