include ../env.mk
include ../Makefile.common

test : all
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) > $(LOG_SCASS) 2> /dev/null
	$(SILENCE)./$(EXE_SC) > $(LOG_SC) 2> /dev/null
	$(SILENCE)if diff $(LOG_SC) $(LOG_SCASS) > /dev/null ; then \
    $(ECHO) -e "$(OK)" ; \
    else $(ECHO) -e "$(KO)" ; \
    fi ;


