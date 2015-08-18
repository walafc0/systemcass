/*------------------------------------------------------------\
  |                                                             |
  | Tool    :                  systemcass                       |
  |                                                             |
  | File    :                 gen_code.cc                       |
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

#if defined(__linux__)
#include <linux/limits.h>
#elif defined(WIN32)
#include <windows.h>
#endif

#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "internal.h"
#include "gen_code.h"
#include "sc_module.h"
#include "sc_ver.h"
#include "process_dependency.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#define casc_cflags GENERATED_MODULE_CFLAGS

// Enable CPP call, this is useful for typeinfo-enabled classes
#define CPP_CALL

using namespace std;

namespace sc_core {

static void PrintCall(std::ostream &, const method_process_t &);
static void open_temp(std::ofstream &, char *);
typedef void (*CASC_ENTRY_FUNC) (void *);
typedef union {
    unsigned long long int integer;
    SC_ENTRY_FUNC pmf;
    CASC_ENTRY_FUNC pf;
} fct;


const char * get_pmf_type() {
    switch (sizeof(SC_ENTRY_FUNC)) {
        case 4:
            // G4 pointer-to-member-function style
            return "unsigned long int";
        case 8:
            // PC pointer-to-member-function style
            return "unsigned long long int";
        default:
            cerr <<
                "Internal Error : Unsupported pointer-to-member-function"
                "(size: " << sizeof(SC_ENTRY_FUNC) << ").\n"
                "Please try --nodynamiclink option.\n";
            exit(21072009);
    };
}


static ostream & operator <<(ostream & o, const SC_ENTRY_FUNC & f) {
    register fct p;
    p.integer = 0;
    p.pmf = f;
    return o << "0x" << hex << p.integer << "ULL";
}


static void PrintCall(std::ostream & o, const method_process_t & m) {
    SC_ENTRY_FUNC func = m.func;
    if (print_schedule) {
        o << "    fprintf(stderr,\"evaluation de " << m.module->name() << "->" << m.name << "()\\n\");\n";
    }
    o << " p.integer = " << func << ";\n";
#ifdef CPP_CALL
    o << " (((sc_module*)(" << m.module << "))->*(p.pmf)) (); /* " << m.module->name() << "->" << m.name << "() */\n";
#else
    o << " p.pf((void *)" << m.module << "); /* " << m.module->name() << "->" << m.name << "() */\n";
#endif
}


static bool is_exist(const char * temp) {
    ifstream o;
    o.open(temp, ios::in);
    if (o.is_open() == false) {
        return false;
    }
    if (o.peek() == -1) {
        return false;
    }
    return true;
}


static void open_temp(ofstream & o, char * temp) {
    /*
       srand (time (NULL));
       int r = rand () % 1000;
       */
    pid_t pid = getpid();
    int r = -1;
    do {
        sprintf(temp, "%s/scheduling-%d-%x.cc", temporary_dir, pid, ++r);
    } while (is_exist(temp));

    o.open(temp, ios::out);
    if (o.is_open() == false) {
        cerr << "Error : Unable to open a file to write scheduling code.\n";
        exit(30032005);
    }
#ifdef CONFIG_DEBUG
    cerr << "opened temporary filename : " << temp << "\n";
#endif
    sprintf(temp, "scheduling-%d-%x", pid, r++);
}


static char * gen_transition(ofstream & o, method_process_list_t & transition_func_list) {
    // transitions
    o << "\ninline void transition(void)\n{\n";
    if (transition_func_list.empty() == false) {
        o << " /* fonctions de transition */\n" << " register fct p;\n";
        method_process_list_t::iterator mm;
        for (mm = transition_func_list.begin(); mm != transition_func_list.end(); ++mm) {
            PrintCall(o, **mm);
        }
    }
    o << "}\n";
}


static char * gen_moore(ofstream & o, method_process_list_t & moore_func_list) {
    // Moore generations (sequential functions)
    o << "\ninline void moore_generation (void)\n{\n";
    if (moore_func_list.empty() == false) {
        o << "  /* fonctions de generation de Moore */\n"
            << " register fct p;\n";
        method_process_list_t::reverse_iterator mm;
        for (mm = moore_func_list.rbegin();
                mm != moore_func_list.rend(); ++mm) {
            PrintCall(o, **mm);
        }
    }
    o << " \n}\n";
}


static char * gen_mealy(ofstream & o, strong_component_list_t & strongcomponents) {
    // Mealy generations (combinational functions only)
    o << "\nextern void mealy_generation (void)\n{\n";
    if (strongcomponents.empty()) {
        return NULL;
    }
    o << "  register fct p;\n" << "\n\n  /* fonctions de mealy */\n";
#ifdef NO_STATIC_SCHEDULE
    o << "\n  do {\n    unstable = 0;\n";
#endif
    strong_component_list_t::iterator ss;
    for (ss = strongcomponents.begin(); ss != strongcomponents.end(); ++ss) {
        if ((*ss)->size() == 1) {
            /* un seul element dans le strong component */
            method_process_t *m = (method_process_t *) (*((*ss)->begin()));
            PrintCall(o, *m);
            continue;
        }
        else {
            /* plusieurs elements dans le strong component */
#ifndef NO_STATIC_SCHEDULE
            o << "\n  do {\n    unstable = 0;\n";
#endif
            component_list_t::reverse_iterator rev_mm;
            for (rev_mm = (*ss)->rbegin(); rev_mm != (*ss)->rend(); ++rev_mm) {
                method_process_t * m = (method_process_t *) * rev_mm;
                PrintCall(o, *m);
            }
#ifndef NO_STATIC_SCHEDULE
            o << "  } while ( unstable );\n\n";
#endif
        }
    }
#ifdef NO_STATIC_SCHEDULE
    o << "  } while ( unstable );\n\n";
#else
    o << "\tunstable = 0;\n";
#endif
}


static char * gen_mealy(ofstream & o, ProcessDependencyList & mealy_func_list) {
    // Mealy generations (combinational functions only)
    o << "\nextern void mealy_generation (void)\n{\n";
    o << "  register fct p;\n" << "\n\n  /* fonctions de mealy */\n";
    ProcessDependencyList::iterator it;
    for (it = mealy_func_list.begin(); it != mealy_func_list.end(); ++it) {
        const method_process_t * m = *it;
        PrintCall(o, *m);
    }
}


char * gen_scheduling_code_for_dynamic_link(method_process_list_t &
        transition_func_list,
        method_process_list_t &
        moore_func_list,
        strong_component_list_t &
        strongcomponents) {
    if (dump_stage) {
        cerr << "Generating C code for scheduling...\n";
    }

    // open temporary file
    ofstream o;
    char base_name[PATH_MAX];
    open_temp(o, base_name);

    if (!o.good()) {
        perror("scheduling: open file\n");
        exit(-1);
    }

    o << "// generated by " << sc_version() << endl
        << "#include <casc.h>\n\n" << "#include <cstdio>\n\n"
        //  << "#include <iostream>\n\n"
        << "namespace sc_core {\n"
        << " typedef void (sc_module::*SC_ENTRY_FUNC)();\n"
        << " typedef void (*CASC_ENTRY_FUNC)(void *);\n";

    const char * pmf_type = get_pmf_type();

    o << " typedef union { "
        << pmf_type
        << " integer; SC_ENTRY_FUNC pmf; CASC_ENTRY_FUNC pf; } fct;\n";

    gen_transition(o, transition_func_list);
    gen_moore(o, moore_func_list);
    gen_mealy(o, strongcomponents);

    o << " \n}\n";
    o << "\n} // end of sc_core namespace\n";

    o.flush();
    o.close();

    // add "cc" extension
    char file_name[PATH_MAX];
    strncpy(file_name, base_name, PATH_MAX);
    file_name[strlen(base_name)] = '\0';
    strcat(file_name, ".cc");
    rename(base_name, file_name);

    if (edit_schedule) {
        run_schedule_editor(file_name);
    }

    if (dump_stage) {
        cerr << "Generating C code for scheduling done.\n";
    }

    return strdup(base_name);
}


char * gen_scheduling_code_for_dynamic_link(method_process_list_t &
        transition_func_list,
        method_process_list_t &
        moore_func_list,
        ProcessDependencyList &
        mealy_func_list) {
    if (dump_stage) {
        cerr << "Generating C code for scheduling...\n";
    }

    // open temporary file
    ofstream o;
    char base_name[PATH_MAX];
    open_temp(o, base_name);

    if (!o.good()) {
        perror("scheduling: open file\n");
        exit(-1);
    }

    o << "// generated by " << sc_version() << endl
        << "#include <casc.h>\n\n" << "#include <cstdio>\n\n"
        << "namespace sc_core {\n"
        << " typedef void (sc_module::*SC_ENTRY_FUNC)();\n"
        << " typedef void (*CASC_ENTRY_FUNC)(void *);\n"
        <<
        " typedef union { unsigned long long int integer; SC_ENTRY_FUNC pmf; CASC_ENTRY_FUNC pf; } fct;\n";

    gen_transition(o, transition_func_list);
    gen_moore(o, moore_func_list);
    gen_mealy(o, mealy_func_list);

    o << "\n}\n";
    o << "\n} // end of sc_core namespace\n";

    o.flush();
    o.close();

    // add "cc" extension
    char file_name[PATH_MAX];
    strncpy(file_name, base_name, PATH_MAX);
    file_name[strlen(base_name)] = '\0';
    strcat(file_name, ".cc");
    rename(base_name, file_name);

    if (edit_schedule) {
        run_schedule_editor(file_name);
    }

    if (dump_stage) {
        cerr << "Generating C code for scheduling done.\n";
    }

    return strdup(base_name);
}


/* base_name est la base du nom du fichier C++
 * casc_cflags est une string qui correspond à $(INCLUDE) d'un Makefile
 */
void compile_code(const char * base_name, const char * casc_cflags2) {
    if (dump_stage) {
        cerr << "Compiling C/C++ code for scheduling...\n";
    }
    char compil_str[512];
    const char * compiler = getenv("CXX");
    const char * systemc_dir = getenv("SYSTEMCASS");
    const char * default_compiler =
#ifdef CPP_CALL
        "g++";
#else
    "gcc";
#endif

    compiler = (compiler == NULL) ? default_compiler : compiler;
    if (systemc_dir == NULL) {
        systemc_dir = getenv("SYSTEMC");
        if (systemc_dir == NULL) {
            cerr << "Error : set SYSTEMCASS or SYSTEMC environnement variable to the SYSTEMCASS directory.\n";
            exit(-1);
        }
    }

    char target_name[128];
    char source_name[128];
    sprintf(target_name, "%s.lo", base_name);
    sprintf(source_name, "%s.cc", base_name);

    if (keep_generated_code) {
        char lg_cde[256];
        sprintf(lg_cde, "mkdir -p %s", generated_files_dir);
        system(lg_cde);
        sprintf(lg_cde, "cp %s/%s %s/", temporary_dir, source_name, generated_files_dir);
        if (dump_stage) {
            cerr << "$ " << lg_cde << "\n";
        }
        system(lg_cde);
        sprintf(lg_cde, "(cd %s ; indent %s)", generated_files_dir, source_name);
        if (dump_stage) {
            cerr << "$ " << lg_cde << "\n";
        }
        system(lg_cde);
    }
    /* ******* */
    /* COMPILE */
    /* ******* */
    const char * commandline_template =
#if defined(CONFIG_OS_DARWIN)
        "(cd %s ;"
        " %s %s -DSCHEDULING_BY_CASC -I%s/include -fno-common -dynamic -o %s -c %s)"
#elif defined(CONFIG_OS_LINUX)
        "(cd %s ; libtool --mode=compile %s %s -DSCHEDULING_BY_CASC -I%s/include -shared -o %s -c %s)"
#else
        "(cd %s ;"
        " %s %s -DSCHEDULING_BY_CASC -I%s/include -dynamiclib -o %s -c %s)"
#endif
        ;

    string cflags = casc_cflags;
    if (use_openmp) {
        cflags += " -fopenmp";
    }

    sprintf(compil_str, commandline_template, temporary_dir, compiler, cflags.c_str(), systemc_dir, target_name, source_name);

    if (dump_stage) {
        cerr << "Executing command : " << compil_str << "\n";
    }

    if (system(compil_str)) {
        perror("compil : system");
        exit(-1);
    }

    /* **** */
    /* LINK */
    /* **** */
    sprintf(target_name, "%s.la", base_name);

#ifdef CONFIG_OS_LINUX
    sprintf(source_name, "%s.lo", base_name);
    sprintf(compil_str, "(cd %s ; pwd ; libtool --mode=link %s %s -module -shared -o %s %s -rpath /tmp)", /* -L. -L%s/lib-%s */
            temporary_dir, compiler, casc_cflags, /*systemc_dir, target_arch, */
            target_name, source_name);
#else
    sprintf(source_name, "%s.o", base_name);
    sprintf(compil_str, "(cd %s ; pwd ; libtool -dynamic -o %s %s)", temporary_dir, target_name, source_name);
#endif

    if (dump_stage) {
        cerr << "Executing command : " << compil_str << "\n";
    }

    if (system(compil_str)) {
        perror("compil : system");
        exit(-1);
    }

    system(compil_str);
    if (dump_stage) {
        cerr << "Compiling done.\n";
    }
}


/* ********************************
 * Function for static scheduling
 */
struct function_call {
    fct * function;
    void ** instance;
    int func_number;
};
static function_call pf[3];


void get_function_call(function_call & pf, method_process_list_t & func_list) {
    pf.func_number = func_list.size();
    pf.function = (fct *) malloc(sizeof(fct) * pf.func_number);
    pf.instance = (void **) malloc(sizeof(void *) * pf.func_number);
    method_process_list_t::iterator mm;
    int i;
    for (mm = func_list.begin(), i = 0; mm != func_list.end(); ++mm, ++i) {
        const method_process_t * mp = *mm;
        pf.function[i].pmf = (mp->func);
        pf.instance[i] = (void *) (mp->module);
    }
}


void get_function_call(function_call & pf, ProcessDependencyList & func_list) {
    pf.func_number = func_list.size();
    pf.function = (fct *) malloc(sizeof(fct) * pf.func_number);
    pf.instance = (void **) malloc(sizeof(void *) * pf.func_number);
    ProcessDependencyList::iterator it;
    int i;
    for (i = 0, it = func_list.begin(); it != func_list.end(); ++it, ++i) {
        const method_process_t * mp = *it;
        pf.function[i].pmf = (mp->func);
        pf.instance[i] = (void *) (mp->module);
    }
}


void gen_scheduling_code_for_static_func(method_process_list_t &
        transition_func_list,
        method_process_list_t &
        moore_func_list,
        ProcessDependencyList &
        mealy_func_list) {
    if (dump_stage) {
        cerr << "Generating scheduling...\n";
    }

    get_function_call(pf[0], transition_func_list);
    get_function_call(pf[1], moore_func_list);
    get_function_call(pf[2], mealy_func_list);

    if (dump_stage) {
        cerr << "Generating scheduling done.\n";
    }
}


void call_functions(function_call & fc) {
    int n = fc.func_number;
    int i;
    for (i = 0; i < n; ++i) {
#if 0
        //defined(CONFIG_DEBUG)
        sc_module *m = (sc_module *) (fc.instance[i]);
        cerr << m->name() << endl;
#endif
        fc.function[i].pf(fc.instance[i]);
    }
}


void call_functions_in_parallel(function_call & fc) {
    int n = fc.func_number;
    int i;
#pragma omp parallel for
    for (i = 0; i < n; ++i) {
#if 0
        //defined(CONFIG_DEBUG)
        sc_module *m = (sc_module *) (fc.instance[i]);
        cerr << m->name() << endl;
        cerr << "thread #" << omp_get_thread_num() << endl;
#endif
        fc.function[i].pf(fc.instance[i]);
    }
}


void static_mealy_generation() {
    call_functions(pf[2]);
}


void static_simulate_1_cycle(void) {
    call_functions(pf[0]); // transition
    update();
    call_functions_in_parallel(pf[1]); // moore generation
    call_functions(pf[2]); // mealy generation
}


/* ***************************************
 * Function for quasi static scheduling
 */

static method_process_list_t func_list[2];
static strong_component_list_t quasistatic_list;

static void Call(const method_process_t & m) {
    sc_module * mod = m.module;
    SC_ENTRY_FUNC func = m.func;
    //  CASC_ENTRY_FUNC   func = reinterpret_cast<CASC_ENTRY_FUNC> (m.func);
    (mod->*func) ();
}


void quasistatic_mealy_generation() {
    strong_component_list_t::iterator ss;
    for (ss = quasistatic_list.begin(); ss != quasistatic_list.end(); ++ss) {
        if ((*ss)->size() == 1) {
            /* un seul element dans le strong component */
            method_process_t * m = (method_process_t *) (*((*ss)->begin()));
            Call(*m);
            continue;
        }
        else {
            /* plusieurs elements dans le strong component */
            do {
                unstable = 0;
                component_list_t::reverse_iterator rev_mm;
                for (rev_mm = (*ss)->rbegin(); rev_mm != (*ss)->rend(); ++rev_mm) {
                    method_process_t * m = (method_process_t *) * rev_mm;
                    Call(*m);
                }
            } while (unstable);
        }
    }
}


void quasistatic_simulate_1_cycle(void) {
    method_process_list_t::iterator mm;
    for (mm = func_list[0].begin(); mm != func_list[0].end(); ++mm) {
        method_process_t & m = **mm;
        Call(m);
    }
    update();
    for (mm = func_list[1].begin(); mm != func_list[1].end(); ++mm) {
        method_process_t & m = **mm;
        Call(m);
    }
    quasistatic_mealy_generation();
}


void gen_scheduling_code_for_quasistatic_func(method_process_list_t &
        transition_func_list,
        method_process_list_t &
        moore_func_list,
        strong_component_list_t &
        mealy_func_list) {
    if (dump_stage) {
        cerr << "Generating quasi static scheduling...\n";
    }

    func_list[0] = transition_func_list;
    func_list[1] = moore_func_list;
    quasistatic_list = mealy_func_list;

    if (dump_stage) {
        cerr << "Generating quasi static scheduling done.\n";
    }
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
