/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 schedulers.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   23_03_2007                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SCHEDULERS_H__
#define __SCHEDULERS_H__

#include "sc_fwd.h"
#include "sc_time.h"

namespace sc_core {

/////////////////////////
///// SYSTEMCASS_SPECIFIC
extern const char * temporary_dir;
extern const char * generated_files_dir;
extern std::string get_scheduling(int scheduling_method);
extern bool        run_schedule_editor(const char * scheduling);

/////////////////////////

} // end of namespace sc_core

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

