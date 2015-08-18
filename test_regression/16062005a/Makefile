
include ../env.mk
HIERARCHY = object_hierarchy.txt
ERASE = $(HIERARCHY)
include ../Makefile.common

HIERARCHY_REF = object_hierarchy_ref.txt

test : $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./$(EXE_SCASS) $(HIERARCHY) 2> /dev/null
	$(SILENCE)if diff $(HIERARCHY) $(HIERARCHY_REF) > /dev/null ; then \
    $(ECHO) -e "$(OK)" ; \
    else $(ECHO) -e "$(KO)" ; \
    fi ;



