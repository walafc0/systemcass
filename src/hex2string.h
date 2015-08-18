/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 hex2string.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef HEX2STRING_H
#define HEX2STRING_H

#include "internal_ext.h"

namespace sc_core {

extern void hex2string(char * buf,const tab_t * val,int bit_number);

} // end of sc_core namespace

#endif

