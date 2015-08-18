/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_object.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_OBJECT_H__
#define __SC_OBJECT_H__

#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
// ----------------------------------------------------------------------------

namespace sc_core {


class sc_object {
	
    ///////////
	// Internal
    // ////////
	protected:
    void init();
    void set_kind(const char *);


    public:
    static const char * const kind_string;
    // LRM
    const char * name() const;
    const char * basename() const;
    void rename(const char*) const;

    friend std::ostream & operator << (std::ostream &, const sc_object &);
    /* virtual */ const char * kind() const;
    /* virtual */ const std::vector<sc_object *> & get_child_objects() const;
    sc_object * get_parent_object() const;

    protected:
    sc_object();
    sc_object(const char *);
    /*virtual */~sc_object();

};

const std::vector<sc_object *> & sc_get_top_level_objects();
const sc_object * sc_find_object(const char *);

} // end of sc_core namespace

using sc_core::sc_get_top_level_objects;
using sc_core::sc_find_object;

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

