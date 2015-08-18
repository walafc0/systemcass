
#include <iostream>
#include <cstring>

#include "systemc.h"
#include "test.h"


using namespace std;


void check_time(int i) {
    const sc_time & t = sc_time_stamp();
    cout << i << endl;

#ifdef SYSTEMCASS_SPECIFIC
    ASSERT((int) (t.to_double()) == i);
#else
    ASSERT((int) (t.to_double()) == i * 1000);
#endif

    cout << t.to_seconds() << endl;
    double seconds = t.to_seconds() * 1000000000;
    cout << seconds << endl;
    ASSERT(((int) seconds) == i);
    char s[256];
    const char * unit;

#ifdef SYSTEMCASS_SPECIFIC
    unit = "NS";
#else
    if (i == 0) {
        unit = "s";
    }
    else {
        unit = "ns";
    }
#endif

    sprintf(s, "%d %s", i, unit);

    ASSERT(strcmp(t.to_string().c_str(), s) == 0); 
}


int sc_main (int argc, char ** argv) {
    sc_clock clk("clock");

    check_time(0);
    sc_start(sc_time(0, sc_core::SC_NS));

    check_time(0);
    sc_start(sc_time(1, sc_core::SC_NS));
    check_time(1);

    sc_start(sc_time(15, sc_core::SC_NS));
    check_time(16);

    sc_start(sc_time(7, sc_core::SC_NS));
    check_time(23);

    sc_start(sc_time(100, sc_core::SC_NS));
    check_time(123);

    sc_start(sc_time(1000, sc_core::SC_NS));
    check_time(1123);

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

