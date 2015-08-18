/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 bit2string.cc                     |
  |                                                             |
  | Author  :                 Kingbo Paul-Jerome                |
  |                           Buchmann Richard                  |
  |                                                             |
  | Date    :                   09_07_2004                      |
  |                                                             |
  \------------------------------------------------------------*/

/* 
 * This file is part of the Disydent Project
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/disydent
 * E-mail             : mailto:richard.buchmann@lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Disydent is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "bit2string.h"
#include "sc_unit.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

static void bit2string_64(char * buf, tab_t * val, int size) {
   typedef sc_dt::s_uint_type<64>::uint_type data_type;
   data_type tmp = *((data_type *) val);
   buf[size] = '\0';
   for (int i = size - 1; i >= 0; --i) {
      buf[i] = (tmp & 1) ? '1' : '0';
      tmp >>= 1;
   }
}

static void bit2string_32(char * buf, tab_t * val, int size) {
   typedef sc_dt::s_uint_type<32>::uint_type data_type;
   data_type tmp = *((data_type *) val);
   buf[size] = '\0';
   for (int i = size - 1; i >= 0; --i) {
      buf[i] = (tmp & 1) ? '1' : '0';
      tmp >>= 1;
   }
}

void bit2string(char * buf, tab_t * val, int bit_number) {
   if (bit_number > 32) {
      bit2string_64(buf, (tab_t *) val, bit_number);
      return;
   }
   else if (bit_number > 16) {
      bit2string_32(buf, (tab_t *) val, bit_number);
      return;
   }
   tab_t tmp = *((tab_t *) val);
   buf[bit_number] = '\0';
   for (int i = bit_number - 1; i >= 0; --i) {
      buf[i] = (tmp & 1) ? '1' : '0';
      tmp >>= 1;
   }

}

char * strip(char * buf) {
   int cpt = 0;
   while ((buf[cpt] == '0') && (buf[cpt + 1] != '\0')) {// tant que bit == 0
      cpt++;
   }
   return (buf + cpt);
}

} // end of sc_core namespace


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

