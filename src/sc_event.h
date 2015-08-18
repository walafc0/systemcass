/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_event.h                        |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_EVENT_H__
#define __SC_EVENT_H__

#include "sc_fwd.h"

#include <iostream>

namespace sc_core {

class sc_event {
    public :
    typedef enum { POS, NEG, VAL } flag_t;

    private:
    const sc_interface & interface;
    flag_t flag;

    public:
    bool pos() const;
    sc_event(const sc_interface &, flag_t);
    ~sc_event();

    // methods
    const sc_interface & get_interface() const {
        return interface;
    } // CASC specific
    flag_t get_flag () const {
        return flag;
    } // CASC specific

    // operators

    friend std::ostream & operator << (std::ostream &, const sc_event &);

    sc_event ();

};

} // end of sc_core namespace

#endif /* __SC_EVENT_H__ */

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

