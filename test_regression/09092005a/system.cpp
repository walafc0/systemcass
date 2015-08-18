
#include <iostream>

#include "systemc.h"
#include "test.h"


using namespace std;

#define ADDRSIZE   32
#define OFFSETSIZE 6
#define BPFSIZE    3


int sc_main (int argc, char ** argv) {

    sc_uint<ADDRSIZE> LINEADDR_MASK = ~(((sc_uint<ADDRSIZE>) ~0x0) >> (ADDRSIZE - OFFSETSIZE - BPFSIZE));
    printf("LINEADDR_MASK == 0x%08x", (unsigned int) LINEADDR_MASK);

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

