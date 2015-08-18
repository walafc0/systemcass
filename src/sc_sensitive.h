/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_sensitive.h                    |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_SENSITIVE_H__
#define __SC_SENSITIVE_H__

#include <list>

#include "sc_fwd.h"
//#include "sc_event.h"
//#include "sc_interface.h"
//#include "internal_ext.h"

namespace sc_core {


// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive
//
//  Static sensitivity class for events.
// ----------------------------------------------------------------------------

class sc_sensitive {
    // from SystemC v201 LRM

    public:
    // constructor, destructor 
    explicit sc_sensitive (sc_module *) {};
    ~sc_sensitive() {};

    public:
    //  specify static sensitivity for processes 
    sc_sensitive & operator () (const sc_event &);
    sc_sensitive & operator () (const sc_interface &);
    sc_sensitive & operator () (const sc_port_base &);
    sc_sensitive & operator () (sc_event_finder &);
    sc_sensitive & operator << (const sc_event &);
    sc_sensitive & operator << (const sc_interface &);
    sc_sensitive & operator << (const sc_port_base &);
    sc_sensitive & operator << (sc_event_finder &);
    
    private:
    // disabled 
    sc_sensitive ();
    sc_sensitive (const sc_sensitive &);
    sc_sensitive & operator = (const sc_sensitive &);
};

// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_pos
//
//  Static sensitivity class for positive edge events.
// ----------------------------------------------------------------------------

class sc_sensitive_pos { /* deprecated */

    public:
    sc_sensitive_pos & operator << (const sc_port_base &);
    sc_sensitive_pos & operator () (const sc_port_base &);
    sc_sensitive_pos & operator << (const sc_interface &);
    sc_sensitive_pos & operator () (const sc_interface &);
};

// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_neg
//
//  Static sensitivity class for negative edge events.
// ----------------------------------------------------------------------------

class sc_sensitive_neg { /* deprecated */

    public:
    sc_sensitive_neg & operator << (const sc_port_base &);
    sc_sensitive_neg & operator () (const sc_port_base &);
    sc_sensitive_neg & operator << (const sc_interface &);
    sc_sensitive_neg & operator () (const sc_interface &);
};


} // end of namespace sc_core

#endif /* __SC_SENSITIVE_H__ */

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

