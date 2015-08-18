/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 serialization.h                   |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   10_01_2006                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SERIALIZATION_H__
#define __SERIALIZATION_H__

#include "serialization_ext.h"
#include "sc_fwd.h"

namespace sc_core {

// SYSTEMCASS SPECIFIC //
extern void set_save_handler(const sc_module &, save_fct_t1);

} // end of sc_core namespace

#endif // __SERIALIZATION_H__

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

