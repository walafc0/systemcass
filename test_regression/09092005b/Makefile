include ../env.mk
include ../Makefile.common

test : $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ; \
	./$(EXE_SCASS) 2> /dev/null ;
	$(SILENCE)if tail -n +8 trace_file.vcd | diff trace_file_reference.vcd - ; then \
		$(ECHO) -e "$(OK)" ; \
	else \
		$(ECHO) -e "$(KO)" ; \
	fi ;


