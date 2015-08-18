
include ../env.mk
SIGNAL_GRAPH = signal_graph.dot
MODULE_GRAPH = module_graph.dot
PORT_GRAPH = port_graph.dot
PROCESS_ORDER = process_order.dot
SIGNAL_ORDER = signal_order.txt
ERASE = $(SIGNAL_GRAPH) $(MODULE_GRAPH) $(PORT_GRAPH) $(PROCESS_ORDER) $(SIGNAL_ORDER)
include ../Makefile.common

MODULE_GRAPH_REF = module_graph_cass_ref.dot
PORT_GRAPH_REF = port_graph_ref.dot
SIGNAL_GRAPH_REF = signal_graph_ref.dot
PROCESS_ORDER_M_REF = process_order_mouchard_ref.dot
PROCESS_ORDER_B_REF = process_order_buchmann_ref.dot

test: $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ; \
   ./$(EXE_SCASS) --a --t --k 2> /dev/null; \
   (tail -n +3 $(MODULE_GRAPH) | diff $(MODULE_GRAPH_REF) -) > /dev/null || res=1 ; \
	./$(EXE_SCASS) --m --t --k 2> /dev/null; \
   (tail -n +3 $(PORT_GRAPH) | diff $(PORT_GRAPH_REF) -) > /dev/null || res=1 ; \
   (tail -n +3 $(SIGNAL_GRAPH) | diff $(SIGNAL_GRAPH_REF) -) > /dev/null || res=1 ; \
	(tail -n +3 $(PROCESS_ORDER) | diff $(PROCESS_ORDER_M_REF) > /dev/null -) || res=1 ; \
	./$(EXE_SCASS) --p --t --k 2> /dev/null; \
   (tail -n +3 $(PORT_GRAPH) | diff $(PORT_GRAPH_REF) -) > /dev/null || res=1 ; \
  	(tail -n +3 $(SIGNAL_GRAPH) | diff $(SIGNAL_GRAPH_REF) -) > /dev/null || res=1 ; \
	(tail -n +3 $(PROCESS_ORDER) | diff $(PROCESS_ORDER_B_REF) -) > /dev/null || res=1 ; \
   if [ x"$$res" = x ] ; then $(ECHO) -e "$(OK)" ; else $(ECHO) -e "$(KO)" ; fi ;


