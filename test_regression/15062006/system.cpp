// QM : je ne sais ce que cherche à tester ce test
// Dans le doute j'applique un traitement par défaut (diff sc) avec un cout "OK"

#include <systemc.h>

#include "test.h"


using namespace std;

template<typename datatype>
class module_base : public sc_module {

    public:
    sc_in_clk clk;
    sc_in<datatype> i1;
    sc_out<datatype> o1;
    sc_signal<datatype> reg1;

    private:
    void trans_module_base() {}
    void gen_module_base() {
        this->kikoo ();
    }
    virtual void kikoo() = 0;


    public:
    SC_HAS_PROCESS(module_base);
    module_base (sc_module_name n) : sc_module (n),
        clk("clk"), i1("i1"), o1("o1"), reg1("reg1") {
        SC_METHOD(trans_module_base);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(gen_module_base);
        sensitive << clk.neg();
        dont_initialize();
    }
};


template<typename datatype>
class test : public module_base<datatype> {

    public:
    sc_in<datatype> i2;
    sc_out<datatype> o2;
    sc_signal<datatype> reg2;

    private:
    void trans() {}

    void gen() {}

    virtual void kikoo() {}

    public:
    SC_HAS_PROCESS(test);
    test(sc_module_name n) : module_base<datatype> (n),
    i2("i2"), 
    o2("o2"),
    reg2("reg2") {
        SC_METHOD(trans);
        this->sensitive << this->clk.pos();
        this->dont_initialize();

        SC_METHOD(gen);
        this->sensitive << this->clk.neg();
        this->dont_initialize();
    }
};


int sc_main(int argc, char * argv[]) {
    sc_clock signal_clk("my_clock");
    sc_signal<int> s01("s01"), s02("s02"), s03("s03"), s04("s04");

    test<int> test1("test1");
    test1.clk(signal_clk);
    test1.i1(s01);
    test1.i2(s02);
    test1.o1(s03);
    test1.o2(s04);

    // Init & run
    sc_start(sc_time(0, sc_core::SC_NS));

    cout << "OK" << endl;
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

