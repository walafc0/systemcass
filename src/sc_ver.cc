/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_ver.cc                         |
|                                                             |
| Author  :                 Taktak Sami                       |
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

#include <iostream>
#include <cstring> //strcmp
#include <cstdlib> //exit

#include "sc_ver.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


namespace sc_core {


static const char copyright[] =
    "        Copyright (c) 2003-2009 by all Contributors\n"
    "                    ALL RIGHTS RESERVED";

static const char systemc_version[] =
    PACKAGE_STRING " --- Compilation date : " __DATE__ " " __TIME__;

static const char splash_screen[] =
    " ____            _                  ____    _    ____ ____  \n"
    "/ ___| _   _ ___| |_ ___ _ __ ___  / ___|  / \\  / ___/ ___| \n"
    "\\___ \\| | | / __| __/ _ \\ '_ ` _ \\| |     / _ \\ \\___ \\___ \\ \n"
    " ___) | |_| \\__ \\ ||  __/ | | | | | |___ / ___ \\ ___) |__) |\n"
    "|____/ \\__, |___/\\__\\___|_| |_| |_|\\____/_/   \\_\\____/____/ \n"
    "       |___/                                                \n"
    "\n"
    "         Cycle Accurate System Simulator\n"
#ifdef CONFIG_DEBUG
    "            DEBUG version\n"
#endif
#ifdef USE_PORT_DEPENDENCY
    "            using explicit port dependancy\n"
#endif
    "            ASIM/LIP6/UPMC\n"
    "            E-mail support:  Richard.Buchmann@asim.lip6.fr\n"
    "            Contributors : Richard Buchmann, Sami Taktak,\n"
    "                           Paul-Jerome Kingbo, Frederic Pétrot,\n"
    "                           Nicolas Pouillon\n"
    "\n"
    "                           Last change : " __DATE__ "\n"
    "\n";


const char * sc_copyright() {
    return copyright;
}


const char * sc_version() {
    return systemc_version;
}


const char * get_splash_screen() {
    return splash_screen;
}


} // end of sc_core namespace


bool casc_check_version(const char * runtime_ver) {
    const char * lib_version = SYSTEMC_VERSION;
    bool right_version = strcmp(runtime_ver, lib_version) == 0;
    if (right_version == false) {
        std::cerr << "Current SystemCASS library version doesn't match.\n";
        std::cerr << "SystemCASS linked library is : " << lib_version << "\n";
        std::cerr << "SystemCASS environnement variable points to : " << runtime_ver << "\n";
        exit(125);
    }

    return true;
}


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

