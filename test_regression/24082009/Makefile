
include ../env.mk
include ../Makefile.common

test : all
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) > $(LOG_SCASS) 2> /dev/null
	$(SILENCE)./$(EXE_SC) > $(LOG_SC) 2> /dev/null
	$(SILENCE)if diff $(LOG_SC) $(LOG_SCASS) > /dev/null ; then \
      time_scass=$$(/usr/bin/time -f "%e" 2>&1 ./$(EXE_SCASS) --nodynamiclink | tail -n 1) ; \
      time_sc=$$(/usr/bin/time -f "%e" 2>&1 ./$(EXE_SC) | tail -n 1) ; \
      $(ECHO) -e "$(OK) (time SystemCass: $$time_scass - time SystemC : $$time_sc)" ; \
   else $(ECHO) -e "$(KO)" ; \
   fi ;
 

