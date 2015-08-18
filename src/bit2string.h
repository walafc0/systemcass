/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 bit2string.h                      |
|                                                             |
| Author  :                 Kingbo Paul-Jerome                |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef BIT2STRING_H
#define BIT2STRING_H

#include "internal.h"

namespace sc_core {

extern void  bit2string(char * buf, tab_t * val, int bit_number);
extern char * strip(char * bitstring);

} // end of sc_core namespace

#endif

