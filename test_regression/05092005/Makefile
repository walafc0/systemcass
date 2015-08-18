
include ../env.mk
include ../Makefile.common


test: $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) 2> /dev/null ;
	$(SILENCE)if [ $$? -eq 0 ] ; then $(ECHO) -e "$(KO)" ; else $(ECHO) -e "$(OK)" ; fi ;


