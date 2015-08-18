/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_module_name.h                  |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_MODULE_NAME_H__
#define __SC_MODULE_NAME_H__

#include "sc_fwd.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_module_name
//
//  Module name class.
// ----------------------------------------------------------------------------

class sc_module_name {

    /////////////////////////
    // SYSTEMCASS SPECIFIC //
    void init();
    /////////////////////////

    const char * m_name;
    bool m_pushed;
    friend class sc_module;

    //  friend class sc_object_manager;
    public:
    sc_module_name(const char *);
    sc_module_name(const sc_module_name &);
    ~sc_module_name();
    operator const char * () const { return m_name; };

    private:
    // disabled 
    sc_module_name();
    sc_module_name & operator = (const sc_module_name &);
};


} // end of namespace sc_core

#endif /* __SC_MODULE_NAME__ */

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

