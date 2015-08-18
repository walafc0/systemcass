/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                   entity.h                        |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                           Taktak Sami                       |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <iostream>
#include <list>
#include "sc_fwd.h"
#include "sc_port.h"
#include "sc_signal.h"

namespace sc_core {

// Entity class
struct entity {
    enum { SIGNAL, PORT } type;
    union {
        // port & signal aren't const because we need to modify
        // some fields in elaboration step.
        sc_port_base * port;
        sc_signal_base * signal;
    };
    sc_object * object;
    sc_interface * interface;
    bool operator ==(const sc_port_base & port_) {
        return (type == PORT) && (&port_ == port);
    }

    bool operator ==(const sc_signal_base & signal_) {
        return (type == SIGNAL) && (&signal_ == signal);
    }

    bool operator <(const entity & e_) const {
        return (void *) this < (void *) &e_;
    } // casting to "unsigned int" causes warnings

    const char * kind() const {
        return object->kind();
    }

    size_t data_size_in_bytes() const {
        return interface->data_size_in_bytes();
    }

    entity(/*const */sc_port_base & port_) {
        type = PORT;
        port = &port_;
        object = &port_;
        interface = &port_;
    }

    entity(/*const */sc_signal_base & signal_) {
        type = SIGNAL;
        signal = &signal_;
        object = &signal_;
        interface = &signal_;
    }
};

// list
typedef std::list < entity > equi_t;
typedef std::list < equi_t > equi_list_t;

// Miscellanous functions
equi_t & get_equi(const sc_interface &);
equi_t & get_equi(const tab_t * pointer);
bool has_equi(/*const */sc_port_base &);
equi_t & merge_equi(const tab_t * pointer);
sc_port_base * get_out_port(const equi_t &);
const char * get_name(const equi_t &);
const char * get_module_name(const equi_t &);
std::ostream & operator <<(std::ostream &, const equi_t &);
std::ostream & operator <<(std::ostream &, const entity &);

const equi_list_t & get_equi_list();
int get_signal_table_size();

// Bind functions
void bind(sc_port_base & p1, sc_port_base & p2);
void bind(sc_port_base & p1, sc_signal_base & s1);

} // end of sc_core namespace

#endif /* __ENTITY_H__ */

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
