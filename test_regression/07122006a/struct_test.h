
#ifndef _STRUCT_TEST_H_
#define _STRUCT_TEST_H_

#include "systemc.h"


struct test : public sc_module {
    sc_in<bool> clk;
    sc_in<bool> resetn;
    sc_out<int> o;
    sc_in <int> i;
    sc_signal<int> reg;

    void transition();
    void gen_moore();
    void gen_mealy();

    SC_CTOR(test) {
        SC_METHOD(transition);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_moore);
        sensitive << clk.neg();
        dont_initialize();
        
        SC_METHOD(gen_mealy);
        sensitive << clk.neg() << i;
        dont_initialize();
    }
};

#endif

