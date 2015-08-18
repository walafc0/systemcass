/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_main.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

/* 
 * This file is part of the Disydent Project
 * Copyright (C) Laboratoire UPMC/LIP6
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

#include <sstream>
#include <list>
#include <set>
#include <cstring> // strcmp
#include <cassert>

#include "internal.h"
#include "global_functions.h"
#include "sc_ver.h"
#include "sc_module.h"
#include "sc_signal.h" // pending_write_vector
#include "dump_dot.h"
#include "dump_used_options.h"
#include "dump_used_env.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


using namespace std;
using namespace sc_core;

typedef list<sc_module *> module_list_t;


namespace sc_core {

bool check_port_dependencies = false;
#ifdef CONFIG_DEFAULT_RUNTIME_COMPILATION
bool dynamic_link_of_scheduling_code = true;
#else
bool dynamic_link_of_scheduling_code = false;
#endif
bool dump_netlist_info    = false;
bool dump_funclist_info   = false;
bool dump_stage           = false;
bool dump_all_graph       = false;
const char * dump_module_hierarchy = NULL;
bool edit_schedule        = false;
bool keep_generated_code  = false;
bool nobanner             = false;
bool noinitialization     = false;
bool nosimulation         = false;
bool notrace              = false;
bool print_schedule       = false;
bool print_user_resources = false;
char * save_on_exit       = NULL;
int  scheduling_method    = NO_SCHEDULING;
bool use_sensitivity_list = false;
bool use_port_dependency  = false;

#ifdef _OPENMP
bool use_openmp = true;
#else
bool use_openmp = false;
#endif


const char * HELP_STRING = \
"\n"
"--a\n"
"       almost static scheduling (use sensitivity list instead of port\n"
"       dependency information\n"
"\n"
"--c\n"
"       print schedule at simulation time (stderr)\n"
"\n"
"--d\n"
"       check port dependencies (stderr)\n"
"\n"
"--edit\n"
"       edit schedule before simulation (run $EDITOR or vim by default)\n"
"\n"
"--f\n"
"       print function list (stderr)\n"
"\n"
"--h\n"
"       display help screen and exit (stdout)\n"
"\n"
"--i\n"
"       print instances list, signals list and statistics if available (stderr)\n"
"\n"
"--k\n"   
"       dump generated scheduling code\n"
"       (generated_by_systemcass/scheduling-xx.cc)\n"
"\n"
"--m\n"
"       Mouchard's static scheduling (use port dependency information instead\n"
"       of sensitivity list)\n"
"\n"
"--modules <filename>\n"
"       dump module hierarchy graph into specified dot file (tons of bugs\n"
"       inside)\n"
"\n"
"--nobanner\n"
"       do not print SystemCASS splash screen\n"
"\n"
"--dynamiclink\n"
"       dynamically link the scheduling code\n"
"\n"
"--nosim\n"
"       run until elaboration stage. Don't simulate\n"
"\n"
"--notrace\n"
"       disable all tracing functions\n"
"\n"
"--p\n"
"       entirely static scheduling (use port dependency information instead of\n"
"       sensitivity list)\n"
"\n"
"--s\n"
"       print stage (stderr)\n"
"\n"
"--save_on_exit <name>\n"
"       save simulation state saved into <name> file when SystemCASS exits\n"
"       (SOCVIEW format)\n"
"\n"
            /* WARNING : we can't avoid destructors execution before saving */
"--t\n"
"       dump either module graph, or signal dependency graph, signal order,\n"
"       and module evalutation order into dot files\n"
"\n"
"--tracestart <n>\n"
"       start tracing functions at #n cycle\n"
"\n"
"--usage\n"
"       print user time elapsed (sec), simulation cycles done (cycles),\n"
"       and simulator performance (cycles/second) (stderr)\n"
"\n"
"--v\n"
"       print internal SystemCASS kernel options (stderr)\n"
"\n";



static void print_splash_screen() {
    // Display once
    if (nobanner == false) {
        cerr << get_splash_screen ();
    }
    nobanner = true;
}


static void check_parameters() {
    if (dump_all_graph) {
        if (use_port_dependency) {
            cerr << "SystemCASS will dump signal dependency graph.\n";
        }
        else {
            cerr << "SystemCASS will dump module dependency graph.\n";
        }
    }
    if (!use_port_dependency && check_port_dependencies) {
        cerr << "Warning : unable to check port dependencies.\n";
    }
    if (!use_port_dependency) {
        use_sensitivity_list = true;
        scheduling_method = CASS_SCHEDULING;
    }
    switch (scheduling_method) {
        case CASS_SCHEDULING :
            assert(use_port_dependency == false);
            break;
        case BUCHMANN_SCHEDULING :
        case MOUCHARD_SCHEDULING :
            if (!use_port_dependency) {
                cerr << "Error : "
                    "The choosen scheduling needs port dependencies informations\n";
                exit (31);
            }
            break;
        default :
            cerr << "Error : You need to choose one of the available scheduling :\n"
                << "- Almost static scheduling like CASS (use sensitivity list)\n"
                << "- Simple static scheduling (use port dependencies)\n"
                << "- Entirely static scheduling (use port dependencies)\n";
            exit (33);
    }
    assert(use_port_dependency || use_sensitivity_list);
}


void apply_parameters(int & argc, char ** &argv) {
#ifdef KEEP_GENERATED_CODE // supprimer scheduling-XXXXXX.cc
    keep_generated_code = true;
#endif
#ifdef DUMP_NETLIST_INFO
    dump_netlist_info = true;
#endif
#ifdef DUMP_FUNCLIST_INFO
    dump_funclist_info = true;
#endif
#ifdef DUMP_STAGE
    dump_stage = true;
#endif
#ifdef DUMP_COMBINATIONAL_LIST2DOT
    dump_all_graph = true;
#endif
#ifdef PRINT_SCHEDULE
    print_schedule = true;
#endif    
#ifdef USE_PORT_DEPENDENCY
    use_port_dependency = true;
#endif
    // parse the command line
    int i;
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                switch (argv[i][2]) {
                    case 'h' : 
                        print_splash_screen();
                        cerr << "Usage : " 
                            << argv[0] << " [--c] [--edit] [--d] [--f] [--h] [--i] [--k] [--modules filename] [--nobanner] [--[no]dynamiclink] [--nosim] [--notrace] [--s] [--t] [--tracestart n] [--usage] [--v] [--p|m|a] [others parameters processed by sc_main]\n"
                            << "Thoses options are processed by SystemCASS library. All the remaining options are passed to sc_main.\n"
                            << "sc_main function retrieves last parameters.\n"
                            << HELP_STRING;
                        noinitialization = true;
                        nosimulation = true;
                        continue;
                    case 'v' : 
                        print_splash_screen ();
                        cerr << get_used_options  () << "\n";
                        cerr << get_used_env () << "\n";
                        cerr << sc_version () << "\n\n";
                        exit (0);
                    case 'u' : 
                        if (strcmp (argv[i] + 2, "usage") == 0) {
                            print_user_resources = true;
                        }
                        else {
                            break;
                        }
                        continue;
                    case 'i' :
                        dump_netlist_info = true;
                        continue;
                    case 'f' :
                        dump_funclist_info = true;
                        continue;
                    case 's' :
                        if (strcmp (argv[i] + 2, "save_on_exit") == 0) {
                            save_on_exit = argv[++i];
                        }
                        else {
                            dump_stage = true;
                        }

                        continue;
                    case 'c' :
                        print_schedule = true;
                        continue;
                    case 'd' :
                        if (strcmp (argv[i] + 2, "dynamiclink") == 0) {
                            dynamic_link_of_scheduling_code = true;
                        }
                        else {
                            check_port_dependencies = true;
                        }
                        continue;
                    case 'e' :
                        if (strcmp (argv[i] + 2, "edit") == 0) {
                            edit_schedule = true;
                        }
                        else {
                            break;
                        }
                        continue;
                    case 'k' :
                        keep_generated_code = true;
                        continue;
                    case 't' :
                        if (strcmp (argv[i] + 2, "tracestart") == 0) {
                            ++i;
                            istringstream iss (argv[i]);
                            iss >> trace_start;
                            trace_start <<= 1;
                            // trace_start = strtoll (argv[i],0,10) << 1;
                            // trace_start = atoll (argv[i]) << 1;
                        }
                        else {
                            dump_all_graph = true;
                        }
                        continue;
                    case 'm' :
                        if (strcmp (argv[i] + 2, "modules") == 0) {
                            ++i;
                            dump_module_hierarchy = argv[i];
                            continue;
                        }
                        else if (strcmp (argv[i] + 2, "m") == 0) {
                            use_port_dependency = true;
                            scheduling_method = MOUCHARD_SCHEDULING;
                            continue;
                        }
                        break;
                    case 'n' :
                        if (strcmp (argv[i] + 2, "nobanner") == 0) {
                            nobanner = true;
                        }
                        else if (strcmp (argv[i] + 2, "nodynamiclink") == 0) {
                            dynamic_link_of_scheduling_code = false;
                        }
                        else if (strcmp (argv[i] + 2, "nosim") == 0) {
                            nosimulation = true;
                        }
                        else if (strcmp (argv[i] + 2, "notrace") == 0) {
                            notrace = true;
                        }
                        else {
                            break;
                        }
                        continue;
                    case 'a' :
                        use_sensitivity_list = true;
                        scheduling_method = CASS_SCHEDULING;
                        continue;
                    case 'p' :
                        use_port_dependency = true;
                        scheduling_method = BUCHMANN_SCHEDULING;
                        continue;
                    default :
                        break;
                }
                break;
            }
        } 
        break;
    }

    // erase SystemCASS options from the command line and give it to the sc_main
    if (i != 1) {
        int j = 1;
        while (i < argc) {
            argv[j++] = argv[i++];
        }
        argc = j;
    }
}


} // end of namespace


using namespace sc_core;

int main(int argc, char * argv[]) {
    apply_parameters(argc, argv);
    print_splash_screen();
    check_parameters();

    if (noinitialization) {
        return 255;
    }

    int ret = sc_main(argc, argv);
    free(pending_write_vector);
    close_systemcass();

    if (have_to_stop) {
        cerr << "'sc_stop' function was called. Exit code : 1\n";
        return 1;
    }

    return ret;
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

