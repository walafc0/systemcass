/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  simplify_string.h                |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   21_09_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef SIMPLIFY_STRING_H
#define SIMPLIFY_STRING_H

#include <string>

// Truncate the name : delete "."
namespace sc_core {

extern std::string & simplify_name(const char * name, std::string & newname);

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

