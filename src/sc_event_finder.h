/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_event_finder.h                 |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_EVENT_FINDER_H__
#define __SC_EVENT_FINDER_H__

#include "sc_fwd.h"

namespace sc_core {

class sc_event_finder {
    /*const*/sc_core::sc_port_base & a_port;

    public:
    sc_event_finder(/*const*/ sc_core::sc_port_base &);
    virtual ~sc_event_finder();

    // methods
    //
    /*const*/ sc_core::sc_port_base& port () /*const*/;

    // operators

    private:
    // disabled
    sc_event_finder();
    sc_event_finder(const sc_event_finder &);
    sc_event_finder & operator = (const sc_event_finder &);
};

}

#endif /* __SC_EVENT_FINDER_H__ */

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

