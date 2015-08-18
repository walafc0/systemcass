#include <signal.h>
#include <iostream>
#include <fstream>
#include <cstdlib> //exit

#include "systemc.h"
#include "test.h"

using namespace std;

struct sc_test : public sc_module {
    sc_in<bool> clk;
    sc_in<bool> resetn;
    sc_signal<sc_uint<6> >  ui6;
    sc_signal<sc_uint<16> > ui16;
    sc_signal<sc_uint<31> > ui31;
    sc_signal<sc_uint<32> > ui32;
    sc_signal<sc_uint<64> > ui64;
    sc_signal<sc_int<6> > i6;
    sc_signal<sc_int<16> > i16;
    sc_signal<sc_int<31> > i31;
    sc_signal<sc_int<32> > i32;
    sc_signal<sc_int<64> > i64;

    unsigned int ui6_c_type : 6;
    sc_uint<6> ui6_sc_uint;

    void eval() {
        if (resetn == false) {
            ui6 = 0xF; ui6_c_type = 0xF;
            ui6_sc_uint = 0xF;
            ui16 = 0x3F;
            ui31 = 0x100;
            ui32 = 0x211;
            ui64 = 0xABCF1234ULL;
            i6 = 0xF;
            i16 = 0x3F;
            i31 = 0x11112222;
            i32 = 0x43214321;
            i64 = 0x4444abcdULL;
        }
        else {
            cout << hex << ui6.read() << " "
                 << ui6_c_type  << " "
                 << ui6_sc_uint << endl;
            ASSERT(ui6.read() == ui6_c_type);
            ui6 = (ui6.read() * 2) + 1;
            ui6_c_type = (ui6_c_type * 2) + 1;
            ui6_sc_uint = (ui6_sc_uint * 2) + 1;
            ui16 = (ui16.read() * 2) + 1;
            ui31 = (ui31.read() * 2) + 1;
            ui32 = (ui32.read() * 2) + 1;
            ui64 = (ui64.read() * 2) + 1;
            i6 = (i6.read() * 2) + 1;
            i16 = (i16.read() * 2) + 1;
            i31 = (i31.read() * 2) + 1;
            i32 = (i32.read() * 2) + 1;
            i64 = (i64.read() * 2) + 1;
        }
    }

    SC_CTOR(sc_test) {
        SC_METHOD(eval);
        sensitive << clk.pos();
        dont_initialize();
    }

};


int sc_main(int argc, char * argv[]) {
    sc_clock clk("clk");
    sc_signal<bool> resetn("resetn");

    sc_test test("test");
    test.clk(clk);
    test.resetn(resetn);

    sc_trace_file * tf;

    tf = sc_create_vcd_trace_file("trace_file");
    sc_trace(tf, test.ui6 , "ui6");
    sc_trace(tf, test.ui16, "ui16");
    sc_trace(tf, test.ui31, "ui31");
    sc_trace(tf, test.ui32, "ui32");
    sc_trace(tf, test.ui64, "ui64");
    sc_trace(tf, test.i6, "i6");
    sc_trace(tf, test.i16, "i16");
    sc_trace(tf, test.i31, "i31");
    sc_trace(tf, test.i32, "i32");
    sc_trace(tf, test.i64, "i64");
    sc_trace(tf, clk, "clk");

    ASSERT(test.ui6.read() == 0);
    ASSERT(test.ui31.read() == 0);
    ASSERT(test.ui32.read() == 0);
    ASSERT(test.ui64.read() == 0);
    ASSERT(test.i6.read() == 0);
    ASSERT(test.i64.read() == 0);

    sc_start(sc_time(0, sc_core::SC_NS));

    resetn = false;

    sc_start(sc_time(3, sc_core::SC_NS));
    resetn = true;
    sc_start(sc_time(20, sc_core::SC_NS));

#if 0
    cout << "0x3f"            << " 0x" << hex << (unsigned int) (sc_uint<6> )test.ui6   << endl;
    cout << "0x100fffff"      << " 0x" << hex << (unsigned int) (sc_uint<31>)test.ui31  << endl;  
    cout << "0x211fffff"      << " 0x" << hex << (unsigned int) (sc_uint<32>)test.ui32  << endl;
    cout << "0xabcf1234fffff" << " 0x" << hex << (long long unsigned int) (sc_uint<64>)test.ui64  << endl;  
#endif
    cout << test.ui6.read() << endl;


    union t {
        int v1:3;
        int v2;
    };


    t t1, t2;
    t1.v1 = 7;
    cout << t1.v1 << " " << t1.v2 << endl;
    t2.v2 = 7;
    cout << t2.v1 << " " << t2.v2 << endl;

    ASSERT(test.ui6.read() == 0x3f);
    ASSERT(test.ui31.read() == 0x100fffff);
    ASSERT(test.ui32.read() == 0x211fffff);

    long long unsigned int ui64_reference = 0xabcf1234ULL;
    ui64_reference <<= 20;
    ui64_reference += 0xfffffULL;

#if 0
    cout << "0xffffffff"      << " 0x" << hex << (int) (sc_int<6>)test.i6    << endl;
    cout << "0x222fffff"      << " 0x" << hex << (int) (sc_int<31>)test.i31   << endl;  
    cout << "0x321fffff"      << " 0x" << hex << (int) (sc_int<32>)test.i32   << endl;
    cout << "0x4444abcdfffff" << " 0x" << hex << (long long signed int) (sc_int<64>)test.i64   << endl;    
#endif

    ASSERT(test.ui64.read() == ui64_reference);
    ASSERT(test.i6.read() == (sc_int<6>) ~ 0);
    ASSERT(test.i31.read() == 0x222fffff);
    ASSERT(test.i32.read() == 0x321fffff);

    long long signed int i64_reference = 0x4444abcdULL;
    i64_reference <<= 20;
    i64_reference += 0xfffffULL;
    ASSERT(test.i64.read() == i64_reference);


    sc_close_vcd_trace_file(tf);

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

