
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> //strcmp

#include <systemc.h>
#include "test.h"


using namespace std;


struct D : sc_module {
    D (sc_module_name n) {}
};


struct C : sc_module {
    C(sc_module_name n) {}
};


struct B : sc_module {
    D d1;
    D d2;
    C c;
    B(sc_module_name n) : d1("d1"), d2("d2"), c("c") {}
};


struct A : sc_module {
    B b;
    C c;
    A(sc_module_name n) : b("b"), c("c") {}
};


struct top_level1 : sc_module {
    A a;
    D d;
    top_level1(sc_module_name insname) : sc_module (insname), a("a"), d("d") {
        ASSERT(strcmp((const char *) insname, "top1") == 0);
    }
};


struct top_level2 : sc_module {
    B b1;
    B b2;
    C c;
    top_level2(sc_module_name insname) : b1("b1"), b2("b2"), c("c") {
        ASSERT(strcmp ((const char *) insname, "top2") == 0);
        ofstream o;
        o.open("results.txt");
        o << (const char *) insname << endl;
        o << insname << endl;
        o.close();
    }
};


int sc_main (int argc, char ** argv) {
    if (argc < 1) {
        cerr << "Usage : " << argv[0] << "\n";
        exit(-1);
    }

    sc_clock clk("clock");
    top_level1 top1("top1");
    top_level2 top2("top2");

    sc_start(sc_time(0, sc_core::SC_NS));

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

