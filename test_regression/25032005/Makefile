
include ../env.mk
SIGNAL_GRAPH = signal_graph.dot
PROCESS_ORDER = process_order.dot
MODULE_GRAPH = module_graph.dot
SIGNAL_ORDER = signal_order.txt
ERASE = $(SIGNAL_GRAPH) $(PROCESS_ORDER) $(SIGNAL_ORDER) $(MODULE_GRAPH)
include ../Makefile.common

SIGNAL_GRAPH_REF = signal_graph_ref.dot
PROCESS_ORDER_REF = process_order_ref.dot
SIGNAL_ORDER_REF = signal_order_ref.txt

test : all
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SC) 2> /dev/null || res=1 ; \
	./$(EXE_SCASS) --p --t 2> /dev/null || res=1 ; \
	(tail -n +3 $(SIGNAL_GRAPH) | diff $(SIGNAL_GRAPH_REF) -) > /dev/null || res=1 ; \
	(tail -n +3 $(PROCESS_ORDER) | diff $(PROCESS_ORDER_REF) -) > /dev/null || res=1 ; \
	diff $(SIGNAL_ORDER_REF) $(SIGNAL_ORDER) > /dev/null || res=1 ;\
	if [ x"$$res" = x ] ; then $(ECHO) -e "$(OK)" ; else $(ECHO) -e "$(KO)" ; fi ;
	
