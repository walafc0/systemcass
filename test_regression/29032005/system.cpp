
#include <signal.h>

#include "systemc.h"
#include "test.h"


using namespace std;

struct M_0i1o : sc_module {
    sc_in_clk clk;
    sc_out<bool> o1;

    void eval() {
        o1 = (rand() % 2) == 0;
    }

    SC_CTOR (M_0i1o) : clk("clk"), o1("o1") {
        SC_METHOD(eval);
        sensitive << clk.neg();
        dont_initialize();
    }

};

struct M_1i1o : sc_module {
    sc_in_clk clk;
    sc_in<bool> i1;
    sc_out<bool> o1;

    void eval1() {
        o1 = ~i1;
    }

    SC_CTOR (M_1i1o) : clk("clk"), i1("i1"), o1("o1") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
    }

};



struct M_2i2o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_out<bool> o1;
    sc_out<bool> o2;

    void eval1() {
        o1 = ~i1;
    }
    void eval2() {
        o2 = ~i2;
    }

    SC_CTOR (M_2i2o) : clk("clk"), i1("i1"), i2("i2"), o1("o1"), o2("o2") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
        SC_METHOD(eval2);
        sensitive << clk.neg();
        sensitive << i2;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o2(i2);
#endif
    }

};


struct M_4i2o : sc_module {
    sc_in_clk clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_in<bool>  i3;
    sc_in<bool>  i4;
    sc_out<bool> o1;
    sc_out<bool> o2;

    void eval1() {
        o1 = ~i1;
    }

    void eval2() {
        o2 = ~i2;
    }

    void eval3() {
        cout << i3 << i4 << "\n";
    }

    SC_CTOR (M_4i2o) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), i4("i4"),
    o1("o1"), o2("o2") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
        SC_METHOD(eval2);
        sensitive << clk.neg();
        sensitive << i2;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o2(i2);
#endif
        SC_METHOD(eval3);
        sensitive << clk.pos();
        dont_initialize();
    }

};


struct M_4i3o : sc_module {
    sc_in_clk    clk;
    sc_in<bool>  i1;
    sc_in<bool>  i2;
    sc_in<bool>  i3;
    sc_in<bool>  i4;
    sc_out<bool> o1;
    sc_out<bool> o2;
    sc_out<bool> o3;

    void eval1() {
        o1 = ~i1;
    }
    
    void eval2() {
        o2 = ~i2;
    }

    void eval3() {
        o3 = ~i3;
    }

    void eval4() {
        cout << i4 << "\n";
    }

    SC_CTOR (M_4i3o) : clk("clk"), i1("i1"), i2("i2"), i3("i3"), i4("i4"), o1("o1"), o2("o2"), o3("o3") {
        SC_METHOD(eval1);
        sensitive << clk.neg();
        sensitive << i1;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o1(i1);
#endif
        SC_METHOD(eval4);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(eval3);
        sensitive << clk.neg();
        sensitive << i3;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o3(i3);
#endif
        SC_METHOD(eval2);
        sensitive << clk.neg();
        sensitive << i2;
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        o2(i2);
#endif
    }
};


struct M_1i0o : sc_module {
    sc_in_clk   clk;
    sc_in<bool> i1;

    void eval1() {
        cout << i1.read() << "\n";
    }

    SC_CTOR (M_1i0o) : clk("clk"), i1("i1") {
        SC_METHOD(eval1);
        sensitive << clk.pos();
        dont_initialize();
    }
};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock",1, 0.5);
    sc_signal<bool> s1("s01"),
        s2 ("s02"),
        s3 ("s03"),
        s4 ("s04"),
        s5 ("s05"),
        s6 ("s06"),
        s7 ("s07"),
        s8 ("s08"),
        s9 ("s09"),
        s10("s10"),
        s11("s11"),
        s12("s12"),
        s13("s13"),
        s14("s14"),
        s15("s15");


    M_0i1o a("a");
    M_4i2o b("b");
    M_1i1o c("c");
    M_4i3o d("d");
    M_1i1o e("e");
    M_1i1o f("f");
    M_2i2o g("g");
    M_1i1o h("h");
    M_1i0o i("i");
    M_0i1o j("j");
    M_0i1o k("k");
    M_0i1o l("l");

    a.clk(signal_clk);
    b.clk(signal_clk);
    c.clk(signal_clk);
    d.clk(signal_clk);
    e.clk(signal_clk);
    f.clk(signal_clk);
    g.clk(signal_clk);
    h.clk(signal_clk);
    i.clk(signal_clk);
    j.clk(signal_clk);
    k.clk(signal_clk);
    l.clk(signal_clk);

    a.o1(s1);
    b.i1(s1);

    b.o2(s2);
    c.i1(s2);

    c.o1(s3);
    d.i1(s3);

    d.o1(s4);
    b.i3(s4);

    d.o2(s5);
    b.i4(s5);

    b.o1(s6);
    e.i1(s6);

    e.o1(s7);
    f.i1(s7);

    f.o1(s8);
    b.i2(s8);

    d.o3(s9);
    g.i1(s9);

    g.o1(s10);
    h.i1(s10);

    h.o1(s11);
    d.i4(s11);

    j.o1(s12);
    g.i2(s12);

    g.o2(s13);
    i.i1(s13);	

    l.o1(s14);
    d.i3(s14);

    k.o1(s15);
    d.i2(s15);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

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

