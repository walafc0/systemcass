/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_module.h                     |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_MODULE_H__
#define __SC_MODULE_H__

#include <list>
#include <set>
#include <stack>

#include "internal.h"
#include "sc_fwd.h"
#include "sc_module_ext.h"
#include "sc_object.h"
#include "sc_sensitive.h"

namespace sc_core {

class sc_port_base;
class sc_signal_base;


//
// INTERNAL IMPLEMENTATION
//
typedef std::list<sc_signal_base *> signal_list_t; 

typedef std::list<sc_module *> instances_list_t; 
typedef std::set<sc_module *> instances_set_t; 
extern instances_set_t instances_set; // ensemble d'instances

typedef std::stack<const sc_module *> modules_stack_t;
extern modules_stack_t modules_stack;
extern void check_all_method_process();
extern void valid_method_process();

extern std::ostream & operator << (std::ostream & o, instances_set_t &);


// ----------------------------------------------------------------------------
//  CLASS : method_process_t
//
//  Method process class.
// ----------------------------------------------------------------------------

class method_process_t {


    public:
    const char * name; // function name

#ifdef DUMP_SCHEDULE_STATS
    long long int number_of_calls;
#endif

    public:
    // data
    sc_module * module;
    SC_ENTRY_FUNC func;
    sensitivity_list_t sensitivity_list;
    bool dont_initialize;

    // constructors
    method_process_t(const char * nm, SC_ENTRY_FUNC fn, sc_module & mod);

    // methods
    friend std::ostream& operator << (std::ostream &, const method_process_t &);

    // accessors
    bool is_combinational(void); 
    bool is_transition(void);
    bool is_genmoore(void);

};


} // end of namespace sc_core

#endif /* __SC_MODULE_H__ */

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

