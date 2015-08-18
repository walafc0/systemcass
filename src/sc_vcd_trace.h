/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_vcd_trace.h                    |
|                                                             |
| Author  :                 Kingbo Paul-Jerome                |
|                           Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef SC_VCD_TRACE_H
#define SC_VCD_TRACE_H

#include "sc_fwd.h"

namespace sc_core {

// Create VCD file
extern sc_trace_file * sc_create_vcd_trace_file(const char * name);
extern void sc_close_vcd_trace_file(sc_trace_file * tf);

} // end of sc_core namespace

using sc_core::sc_create_vcd_trace_file;
using sc_core::sc_close_vcd_trace_file;

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

