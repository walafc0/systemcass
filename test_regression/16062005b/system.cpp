
#include <iostream>
#include <fstream>
#include <vector>

#include "systemc.h"
#include "test.h"

using namespace std;


struct intramoldu : sc_module {
    sc_signal<long> reg_long;
    intramoldu(sc_module_name n) {}
};



struct moldu : sc_module {
    sc_in<bool>             i_bool;
    sc_in<int >             i_int;
    sc_in<sc_int<17> >      i_sc_int17;
    sc_out<int >            o_int;
    sc_out<sc_uint<38> >    o_sc_uint38;
    sc_signal<sc_lv<8> >    reg_lv8;
    sc_signal<int>          reg_int;
    sc_signal<unsigned int> reg_uint;
    sc_signal<sc_uint<3> >  reg_sc_uint3;
    sc_signal<sc_int<31> >  reg_sc_int31;
    sc_in_clk               i_clk;
    intramoldu              microarchitecture;
    moldu(sc_module_name n) :
        i_bool     ("i_bool"),
        i_int      ("i_int"),
        i_sc_int17 ("i_sc_int17"),
        o_int      ("o_int"),
        o_sc_uint38("o_sc_uint38"),
        reg_lv8    ("reg_lv8"),
        reg_int    ("reg_int"),
        reg_uint   ("reg_uint"),
        reg_sc_uint3 ("reg_sc_uint3"),
        reg_sc_int31 ("reg_sc_int31"),
        microarchitecture ("intern_module_of_moldu") {}
};


void dump_objects (ofstream & o, const vector<sc_object *> & obj_list) {
    for (unsigned i = 0; i < obj_list.size(); i++) {
        const sc_object * obj = obj_list[i];
        if (obj == NULL) {
            o << "\nError : NULL pointer in objects list !\n";
        }
        else {
            o << ((i != 0) ? ", " : "") << obj->name();
        }
    }
    o << "\n";
}


int sc_main (int argc, char ** argv) {
    if (argc < 2) {
        cerr << "Usage : " << argv[0] << " <filename>\n";
        exit(-1);
    }

    ofstream o;
    o.open (argv[1], ios::out | ios::trunc);
    if (!o.is_open()) {
        cerr << "Unable to open '" << argv[1] << "'.\n";
        return 1;
    }

    sc_clock s_clk("s_clk");
    sc_signal<bool> s_bool;
    sc_signal<int> s_int;
    sc_signal< sc_int<17> > s_sc_int17;
    sc_signal< sc_uint<38> > s_sc_uint38;
    moldu m("m");
    intramoldu microarchitecture("top_level_module");
    m.i_clk(s_clk);
    m.i_bool(s_bool);
    m.i_int(s_int);
    m.i_sc_int17(s_sc_int17);
    m.o_int(s_int);
    m.o_sc_uint38(s_sc_uint38);

    sc_start(sc_time(0, sc_core::SC_NS));
    o << "Top level :\n";
    dump_objects(o, sc_get_top_level_objects());
    o << "\nChild of \"m\" :\n";
    dump_objects(o, m.get_child_objects());
    o << "\nChild of \"m.intern_module_of_moldu\" :\n";
    dump_objects(o, m.microarchitecture.get_child_objects());
    o << "\nChild of \"top_level_module\" :\n";
    dump_objects(o, microarchitecture.get_child_objects());

    o.close();

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

