
include ../env.mk
include ../Makefile.common

test : all
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) > $(LOG_SCASS) 2> /dev/null
	$(SILENCE)./$(EXE_SC) > $(LOG_SC) 2> /dev/null
	$(SILENCE)if diff $(LOG_SC) $(LOG_SCASS) > /dev/null ; then \
		./$(EXE_SCASS) --p --d 2> /dev/null ; \
		if [ $$? -eq 37 ] ; then \
			$(ECHO) -e "$(OK)" ; \
		else \
			$(ECHO) -e "$(KO) 1" ; \
		fi ; \
	else \
		$(ECHO) -e "$(KO) 2" ; \
	fi ;


