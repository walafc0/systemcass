/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_time.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_TIME_H__
#define __SC_TIME_H__

#include <string>
#include <stdint.h>

namespace sc_core {


// ----------------------------------------------------------------------------
//  ENUM : sc_time_unit
//
//  Enumeration of time units.
// ----------------------------------------------------------------------------

enum sc_time_unit {
    SC_FS = 0,
    SC_PS,
    SC_NS,
    SC_US,
    SC_MS,
    SC_SEC
};


class sc_time;
extern const sc_time SC_ZERO_TIME;
extern sc_time SC_CURRENT_TIME;

extern uint64_t nb_cycles;

inline double sc_simulation_time() {
    // in default time units
    return (double) nb_cycles;
}


const sc_time & sc_time_stamp();



class sc_time {

    friend const sc_time & sc_time_stamp();
    uint64_t time;
    enum sc_time_unit unit;

    public:
    sc_time(double val, sc_time_unit tu);
    sc_time(const sc_time & = SC_ZERO_TIME);

    sc_time & operator= (const sc_time &);

    uint64_t value() const {
        return time;
    }

    inline double to_double() const;
    inline double to_seconds() const;
    inline operator double() const {
        return to_double();
    }

    const std::string to_string() const;

};


inline const sc_time & sc_time_stamp() {
    // in default time units 
    SC_CURRENT_TIME.time = nb_cycles;
    return SC_CURRENT_TIME; // = sc_time (nb_cycles, SC_NS);
}


double sc_time::to_double() const {
    double fact = 1;
    switch (unit) {
        case SC_FS:
            fact = 1e-6;
            break;
        case SC_PS:
            fact = 1e-3;
            break;
        case SC_NS:
            fact = 1;
            break;
        case SC_US:
            fact = 1e3;
            break;
        case SC_MS:
            fact = 1e6;
            break;
        case SC_SEC:
            fact = 1e9;
            break;
    }
    return (double) time * fact;
}


double sc_time::to_seconds() const {
    double fact = 1;
    switch (unit) {
        case SC_FS:
            fact = 1e-15;
            break;
        case SC_PS:
            fact = 1e-12;
            break;
        case SC_NS:
            fact = 1e-9;
            break;
        case SC_US:
            fact = 1e-6;
            break;
        case SC_MS:
            fact = 1e-3;
            break;
        case SC_SEC:
            fact = 1;
            break;
    }
    return (double) time * fact;
}


} // end of namespace sc_core


#endif /* __SC_TIME_H__ */

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

