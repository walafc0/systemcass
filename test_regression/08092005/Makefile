include ../env.mk
include ../Makefile.common

test: $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) --p --t 2> /dev/null ; \
	if [ $$? -eq 197 ] ; then \
		if diff methodprocess_graph.dot methodprocess_graph_reference.dot > /dev/null ; then \
			if tail -n +3 reduced_signal_graph.dot | diff reduced_signal_graph_reference.dot - > /dev/null ; then \
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


