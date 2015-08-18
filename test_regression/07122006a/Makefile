
include ../env.mk
SRC = system_gen_moore.cpp system_gen_mealy.cpp system_transition.cpp
include ../Makefile.common

# Checks compilation always succeeds here)

test : $(EXE_SCASS)
	$(SILENCE)$(ECHO) -ne "| Testing $$(basename $$(pwd)): " ;
	$(SILENCE)./system_systemcass.x > $(LOG_SCASS) 2> /dev/null ; \
    $(ECHO) -e "$(OK)" ; \


