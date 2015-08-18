
#include <systemc.h>
#include <typeinfo>

#include "test.h"


// Tests deactivated (not supported in SystemCass):
// - sc_signal applied to float values (QM)
// Tests failing:
// - sc_int<X> with X different from 16, 32 and 64


using namespace std;

struct test : sc_module {
    int32_t                  reg;
    sc_signal<bool>          reg_bool;
    sc_signal<int>           reg_int;
    sc_signal<unsigned int>  reg_unsigned_int;
    sc_signal<char>          reg_char;
#ifdef ALL_TESTS
    sc_signal<float>         reg_float;
    sc_signal<double>        reg_double;
#endif
    sc_signal<long>          reg_long;
    sc_signal< sc_uint<32> > reg_ui32;
    sc_signal< sc_uint<16> > reg_ui16;
    sc_signal< sc_uint<6> >  reg_ui6;
    sc_signal< sc_int<32> >  reg_i32;
    sc_signal< sc_int<16> >  reg_i16;
    sc_signal< sc_int<6> >   reg_i6;
    sc_in<bool> clk;
    sc_in<bool> resetn;

    void trans() {
        if (resetn.read() == true) {
            //ASSERT(((reg & 1)) == reg_bool.read());
            //ASSERT(((int) reg) == reg_int.read());
            //ASSERT(((unsigned int) reg) == reg_unsigned_int.read());
            //ASSERT(((char) reg) == reg_char.read());
#ifdef ALL_TEST
            //ASSERT(((float) reg) == reg_float.read());
            //ASSERT(((double) reg) == reg_double.read());
#endif
            //ASSERT(((long) reg) == reg_long.read());
            //ASSERT(((unsigned int) reg & 0xFFFFFFFF) == (unsigned int) (reg_ui32.read()));
            //ASSERT(((unsigned int) reg & 0x0000FFFF) == (unsigned int) (reg_ui16.read()));
            //ASSERT(((unsigned int) reg & 0x0000003F) == (unsigned int) (reg_ui6.read()));
            //ASSERT(((signed int) (reg & 0xFFFFFFFF)) == (signed int) (reg_i32.read()));
            //ASSERT(((signed int) (reg & 0x0000FFFF)) == (signed int) (reg_i16.read()));
            //cout << "reg & 0x3F : " << (signed int) (reg & 0x3F) << endl;
            //cout << "reg_i6 : " << reg_i6.read() << endl;
            //ASSERT(((reg & 0x3F) < 32 && ((signed int) (reg & 0x3F)) == (signed int) (reg_i6.read())) || ((reg & 0x3F) >= 32 && (signed int) (reg & 0x3F) != (signed int) (reg_i6.read())));
            cout << "reg_bool : " << reg_bool << endl;
            cout << "reg_int : " << reg_int << endl;
            cout << "reg_unsigned_int : " << reg_unsigned_int << endl;
            cout << "reg_char : " << (unsigned int) reg_char << endl;
#ifdef ALL_TESTS
            cout << "reg_float : " << reg_float << endl;
            cout << "reg_double : " << reg_double << endl;
#endif
            cout << "reg_long : " << reg_long << endl;
            cout << "reg_ui32 : " << reg_ui32 << endl;
            cout << "reg_ui16 : " << reg_ui16 << endl;
            cout << "reg_ui6 : " << reg_ui6 << endl;
            cout << "reg_i32 : " << reg_i32 << endl;
            cout << "reg_i16 : " << reg_i16 << endl;
            cout << "reg_i6 : " << reg_i6 << endl;

            reg = reg + 1;
            reg_bool = reg & 1;
            reg_int = reg;
            reg_unsigned_int = reg;
            reg_char = reg;
#ifdef ALL_TESTS
            reg_float.write(reg);
            reg_double.write(reg);
#endif
            reg_long = reg;
            reg_ui32 = reg;
            reg_ui16 = reg;
            reg_ui6 = reg;
            reg_i32 = reg;
            reg_i16 = reg;
            reg_i6  = reg;
        }
        else {
            reg = 0;
            reg_bool = 0;
            reg_int = 0;
            reg_unsigned_int = 0;
            reg_char = 0;
#ifdef ALL_TESTS
            reg_float = 0;
            reg_double = 0;
#endif
            reg_long = 0;
            reg_ui32 = 0;
            reg_ui16 = 0;
            reg_ui6 = 0;
            reg_i32 = 0;
            reg_i16 = 0;
            reg_i6 = 0;
        }
    }

    SC_HAS_PROCESS(test);
    test(sc_module_name n) : sc_module (n),
    clk("clk") {
        SC_METHOD(trans);
        sensitive << clk.pos();
        dont_initialize();
    }

};


int sc_main (int argc, char * argv[]) {
    sc_clock signal_clk("my_clock", sc_time(1, sc_core::SC_NS));
    sc_signal<bool> resetn("resetn");

    test test1("test1");
    test1.clk(signal_clk);
    test1.resetn(resetn);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

    resetn = false;
    sc_start(sc_time(4, sc_core::SC_NS));
    resetn = true;
    sc_start(sc_time(100, sc_core::SC_NS));

    return EXIT_SUCCESS;
}

#undef sc_inout

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

