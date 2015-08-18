/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_interface.h                    |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_INTERFACE_H__
#define __SC_INTERFACE_H__

#include <stddef.h>

#include "sc_fwd.h"
#include "internal_ext.h"

namespace sc_core {


// ----------------------------------------------------------------------------
//  CLASS : sc_interface
//
//  
// ----------------------------------------------------------------------------

class sc_interface { 
    ///////////
    // Internal
    private:
    tab_t * pointer; // pointeur dans la table de signaux 
                     // NULL pour les sc_out/sc_inout

    public:
    inline tab_t * get_pointer() const { return pointer; }
    inline void set_pointer(tab_t * i) { pointer = i; }
    size_t data_size_in_bytes() const; /* nb of bytes */

    protected:
    void init(size_t) const;

    public:
    const sc_event & default_event() const;
    ~sc_interface();

    protected:
    sc_interface ();

    private:
    // disabled
    // QM: required for compilation...
    sc_interface(const sc_interface &);
    sc_interface & operator = (const sc_interface &);
};

} // end of sc_core namespace

#endif /* __SC_INTERFACE_H__ */

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

