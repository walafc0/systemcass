
include ../env.mk
RES1 = systemcass_a_nodynamiclink_10.txt
RES2 = systemcass_p_nodynamiclink_10.txt
RES3 = systemcass_m_nodynamiclink_10.txt
RES4 = systemcass_dynamiclink_10.txt
ERASE = $(RES1) $(RES2) $(RES3) $(RES4)
include ../Makefile.common

test: $(SC_CASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)(./$(EXE_SCASS) --a --nodynamiclink 10 2> /dev/null ) | tail -n 5 > $(RES1)
	$(SILENCE)(./$(EXE_SCASS) --p --nodynamiclink 10 2> /dev/null ) | tail -n 5 > $(RES2)
	$(SILENCE)(./$(EXE_SCASS) --m --nodynamiclink 10 2> /dev/null ) | tail -n 5 > $(RES3)
	$(SILENCE)(./$(EXE_SCASS) 10 2> /dev/null ) | tail -n 5 > $(RES4)
	$(SILENCE)if diff $(RES1) $(RES2) > /dev/null ; then \
		if diff $(RES1) $(RES3) > /dev/null ; then \
			if diff $(RES1) $(RES4) > /dev/null ; then \
				$(ECHO) -e "$(OK)" ; \
			else \
				$(ECHO) -e "$(KO) 1" ; \
			fi ; \
		else \
			$(ECHO) -e "$(KO) 2" ; \
		fi ; \
	else \
		$(ECHO) -e "$(KO) 3" ; \
	fi ;


