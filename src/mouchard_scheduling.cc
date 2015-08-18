/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                  mouchard_scheduling.cc           |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   20_12_2006                      |
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

#include <set>
#include <iostream>
#include <fstream>

#include "assert.h"
#include "process_dependency.h"
#include "methodprocess_dependency.h"
#include "mouchard_scheduling.h"
#include "simplify_string.h"
#include "sc_fwd.h"
#include "sc_module.h"
#include "sc_ver.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace sc_core {

typedef set<SignalDependency> signalDependencySet;
typedef set<const method_process_t *> processSet;

static void getSource(signalDependencySet &ss, const SignalDependencyGraph &sig_g) {
    typedef set<const equi_t *> nonSource_t;
    nonSource_t nonSource;
    SignalDependencyGraph::iterator it;
    for (it = sig_g.begin(); it != sig_g.end(); ++it) {
        const SignalDependency &sigDep = *it;
        const equi_t * dest = sigDep.destination;
        nonSource.insert(dest);
    }

    for (it = sig_g.begin(); it != sig_g.end(); ++it) {
        const SignalDependency &sigDep = *it;
        const equi_t * src = sigDep.source;
        if (nonSource.find(src) == nonSource.end()) {
            ss.insert(sigDep);
        }
    }
}


static void removeSignals(SignalDependencyGraph &sig_g, signalDependencySet &ss) {
    SignalDependencyGraph::iterator it = sig_g.begin();
    while (it != sig_g.end()) {
        const SignalDependency &sigDep = *it;
        if (ss.find(sigDep) != ss.end()) {
            SignalDependencyGraph::iterator jt = it++;
            sig_g.erase(jt);
        }
        else {
            ++it;
        }
    }
}


ProcessDependencyList * MakeMouchardScheduling(const SignalDependencyGraph & _sig_g) {
    if (dump_stage) {
        cerr << "Making process dependency list...\n";
    }
    ProcessDependencyList * mod_l = new ProcessDependencyList ();
    SignalDependencyGraph sig_g = _sig_g;
    while (!sig_g.empty ()) {
        signalDependencySet ss;
        getSource(ss, sig_g);
        removeSignals(sig_g, ss);
        processSet ps;
        signalDependencySet::iterator sit;
        for (sit = ss.begin(); sit != ss.end(); ++sit) {
            const SignalDependency & sigdep  = *sit;
            const method_process_t * process = sigdep.method;
            ps.insert(process);
        }
        processSet::iterator pit;
        for (pit = ps.begin(); pit != ps.end(); ++pit) {
            const method_process_t * process = *pit;
            mod_l->push_back(process);
            if (dump_stage) {
                cerr << "Process found : " << *process << "\n";
            }
        }
    }

    return mod_l;
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

