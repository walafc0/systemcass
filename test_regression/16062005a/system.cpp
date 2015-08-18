
#include <iostream>
#include <fstream>
#include <vector>

#include "systemc.h"
#include "test.h"


using namespace std;

struct D : sc_module {
    D(sc_module_name) {}
};



struct C : sc_module {
    C(sc_module_name) {}
};


struct B : sc_module {
    D d1;
    D d2;
    C c;
    B(sc_module_name) : d1("d1"), d2("d2"), c("c") {}
};


struct A : sc_module {
    B b;
    C c;
    A(sc_module_name) : b("b"), c("c") {}
};


struct top_level1 : sc_module {
    A a;
    D d;
    top_level1(sc_module_name) : a("a"), d("d") {}
};


struct top_level2 : sc_module {
    B b1;
    B b2;
    C c;
    top_level2(sc_module_name) : b1("b1"), b2("b2"), c("c") {}
};


void dump_hierarchy(ostream & o, sc_object * obj) {
    const vector<sc_object *> & children = obj->get_child_objects();
    for (unsigned i = 0; i < children.size(); i++) {
        if (children[i]) {
            dump_hierarchy(o, children[i]);
        }
    }
    o << obj->name() << " " << obj->kind() << endl;
}


void dump_hierarchy (ostream & o, const vector<sc_object *> & obj_list) {
    for (unsigned i = 0; i < obj_list.size(); i++) {
        dump_hierarchy(o, obj_list[i]);
    }
}


int sc_main (int argc, char ** argv) {
    if (argc < 2) {
        cerr << "Usage : " << argv[0] << " <filename>\n";
        exit(-1);
    }

    sc_clock clk("clock");
    top_level1 top1("top1");
    top_level2 top2("top2");

    ofstream o;
    o.open(argv[1],ios::out | ios::trunc);
    if (!o.is_open()) {
        cerr << "Unable to open '" << argv[1] << "'.\n";
        return 1;
    }

    sc_start(sc_time(0, sc_core::SC_NS));
    dump_hierarchy(o, sc_get_top_level_objects());

    ASSERT(sc_find_object("top2.b1"    ) == &top2.b1    );
    ASSERT(sc_find_object("top2.b1.d1" ) == &top2.b1.d1 );
    ASSERT(sc_find_object("top2.b1.d2" ) == &top2.b1.d2 );
    ASSERT(sc_find_object("top2.b1.c"  ) == &top2.b1.c  );
    ASSERT(sc_find_object("top2.b2"    ) == &top2.b2    );
    ASSERT(sc_find_object("top2.c"     ) == &top2.c     );
    ASSERT(sc_find_object("top1.a"     ) == &top1.a     );
    ASSERT(sc_find_object("top1.d"     ) == &top1.d     ); 
    ASSERT(sc_find_object("top1.a.b"   ) == &top1.a.b   );
    ASSERT(sc_find_object("top1.a.b.d1") == &top1.a.b.d1);
    ASSERT(sc_find_object("top1.a.b.d2") == &top1.a.b.d2);
    ASSERT(sc_find_object("top1.a.b.c" ) == &top1.a.b.c );
    ASSERT(sc_find_object("top1.a.c"   ) == &top1.a.c   );  
    ASSERT(sc_find_object("top1.c"     ) == NULL        );
    ASSERT(sc_find_object("top1"       )->get_parent_object() == NULL);
    ASSERT(sc_find_object("top1.a"     )->get_parent_object() == &top1);
    ASSERT(sc_find_object("top1.a.b"   )->get_parent_object() == &top1.a);
    ASSERT(sc_find_object("top1.a.b.d2")->get_parent_object() == &top1.a.b);
    ASSERT(sc_find_object("top1.d"     )->get_parent_object() == &top1);
    ASSERT(sc_find_object("top2.b1"    )->get_parent_object() == &top2);
    ASSERT(sc_find_object("top2.b2"    )->get_parent_object() == &top2);
    ASSERT(sc_find_object("top2.c"     )->get_parent_object() == &top2);
    ASSERT(sc_find_object("top2.b1.c"  )->get_parent_object() == &top2.b1);

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

