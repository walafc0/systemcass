
include ../env.mk
RESULTS = results.txt
ERASE = $(RESULTS)
include ../Makefile.common

RESULTS_REF = results_ref.txt

test : $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) 2> /dev/null
	$(SILENCE)if diff $(RESULTS) $(RESULTS_REF) > /dev/null ; then \
    $(ECHO) -e "$(OK)" ; \
    else $(ECHO) -e "$(KO)" ; \
    fi ;



