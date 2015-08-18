/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 casc.h                            |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/

#ifndef __CASC_H__
#define __CASC_H__

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#include <cstdio>
#include <stdint.h>

EXTERN char unstable;
EXTERN int32_t pending_write_vector_nb;

namespace sc_core {
    struct sc_module;
    inline void transition        (void);
    EXTERN void update            (void);
    inline void moore_generation  (void);
    EXTERN void mealy_generation  (void);
    EXTERN bool casc_check_version(const char *);
}

#ifdef SCHEDULING_BY_CASC
#include <systemcass_version_ext.h>

namespace sc_core {

    EXTERN void initialize() {
        casc_check_version(SYSTEMC_VERSION);
    }

    EXTERN void simulate_1_cycle(void) {
        transition();
        update();
        moore_generation();
        mealy_generation(); 
    }
} // end of sc_core namespace

#endif

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

