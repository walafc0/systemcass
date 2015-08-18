
#include "systemc.h"
#include "test.h"


using namespace std;


struct inner : sc_module {
    sc_in_clk clk;
    sc_in<int> i1;
    int reg;

    void save_state (FILE * fic) {
        cerr << "saving " << name() << "\n";
        fprintf (fic, "%d\n", reg);
    }

    void restore_state (FILE * fic) {
        cerr << "restoring " << name() << "\n";
        int i;
        fscanf (fic, "%d\n", &i);
        reg = i;
    }

    void trans() {
        reg = i1.read();
    }

    SC_HAS_PROCESS(inner);
    inner(sc_module_name n) : sc_module(n),
    clk("clk"),
    i1("i1") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        SAVE_HANDLER(save_state);
#endif
    }

};


struct test : sc_module {
    sc_in_clk clk;
    sc_in<bool> i1;
    sc_in<int> i2;
    sc_in<int> i3;
    inner inner1;

    int tab[16];
    bool b;

    void trans() {
        b = i1.read() ^ b;
        tab[i3.read() % 16] = i2;
    }

    void save_state (FILE * fic) {
        cerr << "saving " << name() << "\n";
        fprintf(fic, "%c\n", ((b) ? '1' : '0'));
        int i;
        for (i = 0; i < 16; ++i) {
            fprintf(fic, "%d\n", tab[i]);
        }
    }

    void restore_state(FILE * fic) {
        cerr << "restoring " << name() << "\n";
        int j;
        fscanf (fic, "%d\n", &j);
        b = (j > 0);
        int i;
        for (i = 0; i < 16; ++i) {
            fscanf (fic, "%d\n", &j);
            tab[i] = j;
        }
    }

    SC_HAS_PROCESS(test);
    test (sc_module_name n) : sc_module (n),
    clk("clk"),
    i1("i1"), i2("i2"), i3("i3"),
    inner1 ("inner1") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();
#ifdef SYSTEMCASS_SPECIFIC
        SAVE_HANDLER(save_state);
#endif
    }

};


int usage(const char * com) {
    cout << "Usage :\n" << com << " [#cycles]\n";
    return EXIT_FAILURE;
}

sc_signal<bool> s01("bool");
sc_signal<int> s02("tab_index"), s03("value_to_write_in_tab");


void * func() {
    cerr << "func () at #" << sc_time_stamp() << endl;
    int i = (int)(sc_time_stamp().to_double()) / 1000;
    s01  = (i & 1) > 0;
    s02  = (i + (i << 1)) << 6;
    s03  = i;
    ++i;
    return 0;
}


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock", 1, 0.5);

    test test1("test1");
    test1.clk(signal_clk);
    test1.i1(s01);
    test1.i2(s02);
    test1.i3(s03);
    test1.inner1.clk(signal_clk);
    test1.inner1.i1(s02);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

#ifndef SOCVIEW
    if (argc != 2) {
        return usage(argv[0]);
    }

    int nb = atoi(argv[1]);

    if (nb == 0) {
        return usage(argv[0]);
    } 

    int i = 0;
    while (i++ < nb) {
        func();
        sc_start(sc_time(1, sc_core::SC_NS));
    }
#else
    debug(&func); 
#endif

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

