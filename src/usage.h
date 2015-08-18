/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                  usage.h                          |
  |                                                             |
  | Author  :                 Buchmann Richard                  |
  |                                                             |
  | Date    :                   ??_??_2005                      |
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

#ifndef __USAGE_H__
#define __USAGE_H__

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <iostream>

struct usage_t {
    unsigned int elapsed_time;
    long memory_size;
    usage_t() {
        elapsed_time = 0;
        memory_size  = 0;
    }

    void start() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        elapsed_time = usage.ru_utime.tv_sec;
        memory_size = usage.ru_maxrss;
    }

    void cont() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        elapsed_time = usage.ru_utime.tv_sec - elapsed_time;
        memory_size = usage.ru_maxrss;
    }

    void stop() {
        cont();
    }

    operator unsigned int () const {
        return elapsed_time;
    }

    long get_memory_size () const {
        return memory_size;
    }

    friend std::ostream & operator << (std::ostream & o, const usage_t & c) {
        return o << c.elapsed_time << "seconds";
    }

};

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

