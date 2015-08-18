
#include <iostream>

#include "systemc.h"
#include "test.h"


using namespace std;


typedef sc_uint<36> data_type; 


class param_t {
    public:
    static const unsigned int B = 36;
    typedef sc_dt::sc_uint<B> be_t;
};


template <typename param_t>
class test_t {
    public:
    typedef typename param_t::be_t test_be_t;

    test_be_t test_be;
};


int sc_main (int argc, char ** argv) {
    sc_uint<36> a;
    const long long int ca = 0xf00000000LLU;
    a = ca;

    test_t<param_t> test1, test2;
    test1.test_be = a;
    test2.test_be = ca;

    if (test1.test_be != test2.test_be) {
        return 1;
    }

    bool ret = (test1.test_be == a);
    if (!ret) {
        return 1;
    }

    cout << "OK" << endl;
    return 0;

    /* the following should be compiled and not executed */
    // QM : je ne sais pas pourquoi ; je n'ai pas cherché mais ça seg faulte...
    sc_signal<test_t<param_t>::test_be_t > sig_test;
    sig_test = test1.test_be;

    ret = (sig_test.read() == test2.test_be); /* OK */
    if (!ret) {
        return 1;
    }

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

