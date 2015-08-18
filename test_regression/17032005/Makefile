
include ../env.mk
TRACE1 = trace_file_scass.vcd
TRACE2 = trace_file_systemc.vcd
ERASE = $(TRACE1) $(TRACE2)
include ../Makefile.common

TRACE = trace_file.vcd
TRACE_REF = trace_file_reference.vcd

test: all
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) > $(LOG_SCASS) 2> /dev/null
	$(SILENCE)mv $(TRACE) $(TRACE1)
	$(SILENCE)./$(EXE_SC) > $(LOG_SC) 2> /dev/null
	$(SILENCE)mv $(TRACE) $(TRACE2)
	$(SILENCE)if tail -n +8 $(TRACE1) | diff $(TRACE_REF) - > /dev/null ; then \
		if tail -n +8 $(TRACE2) | diff $(TRACE_REF) - > /dev/null ; then \
			$(ECHO) -e "$(OK)" ; \
		else \
			$(ECHO) -e "$(KO) 1" ; \
		fi ; \
	else \
		$(ECHO) -e "$(KO) 2" ; \
	fi ;



