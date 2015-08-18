
#include "systemc.h"

#include "test.h"

/*
 * Test for detecting an overflow in an array of sc_signal.
 * I (QM) am not sure it is possible to detect that actually...
 */

using namespace std;

struct hard : sc_module {
    sc_in<bool> i;
    sc_out<bool> o;
    SC_HAS_PROCESS(hard);
    hard(sc_module_name) {}
};


int sc_main (int argc, char ** argv) {
    sc_clock signal_clk("my_clock", sc_time(1, sc_core::SC_NS));
    sc_signal<bool> s[5];
    hard a("a");
    hard b("b");
    a.i(s[0]);
    a.o(s[1]);
    b.i(s[5]); // Wrong array index
    b.o(s[2]);

    sc_start(sc_time(0, sc_core::SC_NS));

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

