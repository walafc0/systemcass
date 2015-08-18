
include ../env.mk
include ../Makefile.common


test: $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$< 2> /dev/null ; \
	if [ $$? -eq 165 ] ; then $(ECHO) -e "$(OK)" ; else $(ECHO) -e "$(KO)" ; fi ;


