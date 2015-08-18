/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_localvar.h                   |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   21_04_2005                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef SC_LOCALVAR_H
#define SC_LOCALVAR_H

#include "sc_fwd.h"
#include "sc_signal.h"

namespace sc_core {

template <typename T>
class sc_localvar : public sc_signal_base {

    public:
    sc_localvar (const T & object) {
        init (object);
    }

    private:
    void init (const T & object) {
        set_pointer ((tab_t *) &object);
        set_kind ("sc_localvar");
        sc_interface::init (sizeof(object));
        bind (*((sc_signal_base *) this));
    }

};

} // end of sc_core namespace

#endif

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

