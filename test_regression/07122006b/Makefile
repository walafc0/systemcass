include ../env.mk
include ../Makefile.common

# Checks compilation, always succeeds here

test : $(EXE_SCASS)
	@echo -ne "| Testing $$(basename $$(pwd)): " ;
	@./$(EXE_SCASS) > $(LOG_SCASS) 2> /dev/null
	@echo -e "$(OK)" ; \
	

