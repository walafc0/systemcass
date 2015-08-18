
sclibdir = $(prefix)/lib-@SC_LIB_TARGET@

sclib_LTLIBRARIES = libsystemc.la

libsystemc_objects= alias.o bit2string.o dump_dot.o dump_used_env.o     \
dump_used_options.o entity.o global_functions.o graph.o graph_cass.o    \
graph_signals.o hex2string.o methodprocess_dependency.o                 \
module_hierarchy.o module_hierarchy2dot.o mouchard_scheduling.o         \
port_dependency.o process_dependency.o sc_clock.o sc_event.o            \
sc_event_finder.o sc_interface.o sc_main.o sc_module.o sc_numrep.o      \
sc_object.o sc_pat_trace.o sc_port.o sc_sensitive.o sc_time.o           \
sc_trace.o sc_uint_subref_r.o sc_vcd_trace.o sc_ver.o schedulers.o      \
serialization.o signal_dependency.o simplify_string.o
# sc_logic.o

libsystemc_headers= graph_cass.h sc_biguint.h sc_nbdefs.h               \
sc_unsigned.h alias.h sc_bit.h graph_signals.h sc_bv.h sc_numrep.h      \
sc_vcd_trace.h bit2string.h sc_clock.h sc_object.h sc_ver.h casc.h      \
hex2string.h sc_clock_ext.h sc_ver_ext.h data_field.h internal.h        \
sc_pat_trace.h internal_ext.h sc_event.h schedulers.h dump_dot.h        \
sc_port.h methodprocess_dependency.h sc_event_finder.h sc_port_ext.h    \
serialization.h dump_used_env.h sc_fwd.h serialization_ext.h            \
module_hierarchy.h sc_int.h sc_sensitive.h dump_used_options.h          \
sc_signal.h signal_dependency.h module_hierarchy2dot.h sc_interface.h   \
sc_signed.h entity.h module_hierarchy_ext.h sc_localvar.h sc_string.h   \
simplify_string.h fsm_rules.h systemc mouchard_scheduling.h sc_logic.h  \
sc_time.h systemc.h gen_code.h sc_lv.h systemcass_version_ext.h         \
port_dependency.h sc_trace.h usage.h global_functions.h                 \
port_dependency_ext.h sc_trace_ext.h utils.h sc_module.h sc_uint.h      \
graph.h process_dependency.h sc_module_ext.h sc_bigint.h                \
sc_module_name.h sc_unit.h

libsystemc_la_SOURCES = $(libsystemc_objects:.o=.cc) $(libsystemc_headers)

if WITH_ALLIANCE
ALLIANCE_LIBS = -L$(ALLIANCE_PATH)/lib -lMut -lPat -lPpt -lPgn
endif

libsystemc_la_LIBADD = libgen_code.la $(ALLIANCE_LIBS)
libsystemc_la_CXXFLAGS = @ALLIANCE_CFLAGS@ @OPENMP_CFLAGS@ $(CXXFLAGS)

noinst_LTLIBRARIES =  libgen_code.la
libgen_code_la_SOURCES = gen_code.cc
libgen_code_la_CXXFLAGS = -DGENERATED_MODULE_CFLAGS='"$(CXXFLAGS) -I$(prefix)/include"' @OPENMP_CFLAGS@

include_HEADERS = alias.h casc.h data_field.h global_functions.h internal_ext.h module_hierarchy_ext.h port_dependency_ext.h sc_bigint.h sc_biguint.h sc_bit.h sc_bv.h sc_clock_ext.h sc_event.h sc_fwd.h sc_int.h sc_interface.h sc_localvar.h sc_logic.h sc_lv.h sc_module_ext.h sc_module_name.h sc_nbdefs.h sc_numrep.h sc_object.h sc_pat_trace.h sc_port_ext.h sc_sensitive.h sc_signal.h sc_signed.h sc_string.h sc_time.h sc_trace_ext.h sc_uint.h sc_unit.h sc_unsigned.h sc_vcd_trace.h sc_ver_ext.h serialization_ext.h systemc systemc.h systemcass_version_ext.h

