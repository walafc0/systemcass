/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 gen_code.h                        |
  |                                                             |
  | Author  :                 Taktak Sami                       |
  |                           Buchmann Richard                  |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/
#ifndef __GEN_CODE_H__
#define __GEN_CODE_H__

#include "internal.h"
#include "global_functions.h"
#include "graph.h"
#include "sc_port.h"
#include "sc_trace.h"
#include "process_dependency.h"

//-------------------------------------------------------------------
#ifdef __GNUC__
#define INLINE __attribute__((always_inline))
#else
#define INLINE
#endif

//-------------------------------------------------------------------
namespace sc_core {

extern strong_component_list_t * strong;

/* compile scheduling code to link dynamically later */
extern void compile_code(const char * base_name, const char * cflags2 = "");

/* generate a scheduling code */
extern void  gen_scheduling_code_for_quasistatic_func(
      method_process_list_t   &transition_list,
      method_process_list_t   &moore_list,
      strong_component_list_t &mealy_list);

extern void  gen_scheduling_code_for_static_func(
      method_process_list_t   &transition_list,
      method_process_list_t   &moore_list,
      ProcessDependencyList   &mealy_list);

extern char * gen_scheduling_code_for_dynamic_link(
      method_process_list_t   &transition_list,
      method_process_list_t   &moore_list,
      ProcessDependencyList   &mealy_list);

extern char * gen_scheduling_code_for_dynamic_link(
      method_process_list_t   &transition_list,
      method_process_list_t   &moore_list,
      strong_component_list_t &strongcomponents);

/* function when any dynamic link is impossible */
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void static_simulate_1_cycle();
EXTERN void static_mealy_generation();
EXTERN void quasistatic_simulate_1_cycle();
EXTERN void quasistatic_mealy_generation();

/* internal functions */
inline void switch_to_moore() INLINE;
inline void internal_sc_cycle2() INLINE;
inline void internal_sc_cycle1(int number_of_cycles) INLINE;
inline void internal_sc_cycle0(double duration)  INLINE;

/* ***************** */
/* inlined functions */
/* ***************** */

inline void internal_sc_cycle2() {
#ifdef DUMP_STAGE
    std::cerr << "begin of cycle #" << sc_simulation_time () << "\n";
#endif
    func_simulate_1_cycle();  
    ++nb_cycles;
#ifdef DUMP_STAGE
    std::cerr << "end of cycle\n";
#endif
}


inline void internal_sc_cycle1(int number_of_cycles) {  
    //while ((! have_to_stop) && (number_of_cycles != 0)) {
    while (!((have_to_stop) | (number_of_cycles == 0))) {
        trace_all(false);
        internal_sc_cycle2();
        trace_all(true);
        number_of_cycles = (number_of_cycles<0) ? number_of_cycles:number_of_cycles - 1;
    }
}


inline void internal_sc_cycle0(double duration) {
    // in default time units

#ifdef CONFIG_DEBUG
    // Check dynamic linkage
    if ((func_combinationals == NULL) || (func_simulate_1_cycle == NULL)) {
        std::cerr << "Main execution loop is not yet generated.\n";
    }

    if (duration < -1) {
        std::cerr << "Invalid duration.\n";
    }
#endif

    if (is_posted_write ()) {
        // update posted value to external signals		
        update ();
        func_combinationals ();
    }

    internal_sc_cycle1 ((int) duration);

    // don't need to do func_combinationals since 'unstable' flag is now false
    if (is_posted_write()) {
        update();
        func_combinationals();
    }
}

//-------------------------------------------------------------------

// sc_cycle is for internal purpose only.
// sc_cycle is deprecated since 1.0
//extern void sc_cycle( double duration );  // in default time units

/* time_unit is worth a cycle in every cases */
//extern void sc_cycle( double duration, sc_time_unit time_unit );

} // end of sc_core namespace

#endif /* __GEN_CODE_H__ */

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

