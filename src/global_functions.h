/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 global_functions.h                |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/
#ifndef __GLOBAL_FUNCTIONS_H__
#define __GLOBAL_FUNCTIONS_H__

#include "sc_fwd.h"
#include "sc_time.h"

extern int sc_main(int, char **);

namespace sc_core {

/////////////////////////
enum sc_stop_mode
{
    SC_STOP_FINISH_DELTA,
    SC_STOP_IMMEDIATE
};
/////////////////////////

extern int main(int argc, char * argv[]);

extern void sc_initialize(void);

extern void sc_start(double d_val)  __attribute__((deprecated));
extern void sc_start();
extern void sc_start(const sc_time & duration );
extern void sc_start(double d_val, sc_time_unit d_tu);
extern void sc_stop();
extern void sc_set_stop_mode (sc_stop_mode);
extern sc_stop_mode sc_get_stop_mode ();

extern const char * sc_gen_unique_name(const char * basename_);

/////////////////////////
///// SYSTEMCASS_SPECIFIC

extern void close_systemcass();
extern bool run_schedule_editor(const char *);
extern const char * temporary_dir;
extern const char * generated_files_dir;

/////////////////////////

} // end of namespace sc_core

using sc_core::sc_gen_unique_name;
using sc_core::sc_initialize;
using sc_core::sc_start;
using sc_core::sc_stop;

/////////////////////////

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

