/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_fwd.h                          |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_FWD_H__
#define __SC_FWD_H__

#include <vector>
//#include <list>

namespace sc_core {

/////////////////////////////////////////////////
// kernel
/////////////////////////////////////////////////
class method_process_t;
class sc_port_base;
class sc_object;
class sc_interface;
class sc_module;
class sc_event;
class sc_event_finder;
class sc_sensitive;
class sc_sensitive_pos;
class sc_sensitive_neg;
class sc_module_name;
class sc_time;

/////////////////////////////////////////////////
// Ports & Signals
/////////////////////////////////////////////////
template <class T> class sc_in;
template <class T> class sc_inout;
template <class T> class sc_out;
template <class T> class sc_signal;
template < typename T > class sc_port_b;
class sc_signal_base;
class sc_clock;

/////////////////////////////////////////////////
// Tracing
/////////////////////////////////////////////////
class sc_trace_file;

/////////////////////////////////////////////////
// lists
/////////////////////////////////////////////////
typedef std::vector<method_process_t *> method_process_list_t;

}


/////////////////////////////////////////////////
// Data Types
/////////////////////////////////////////////////
namespace sc_dt {

template <int W> class sc_bigint;
template <int W> class sc_biguint;
template <int W> class sc_int;
template <int W> class sc_uint;
template <int W> class sc_bv;
template <int W> class sc_lv;
class sc_unsigned;
class sc_signed;
class sc_bit;
class sc_logic;

}

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

