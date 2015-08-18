# Number    : Notes
# 17032005  : trace & basic arithmetic using sc_uint/sc_int types
# 25032005  : check signal graph, module order, signal order (--p --t)
#             simple architecture
# 29032005  : check signal graph, module order, signal order (--p --t)
#             advanced architecture
# 30032005  : check signal graph, module order, signal order (--p --t)
#             advanced architecture
#             bit mask
# 19042005  : check vcd trace 
#             (bool, char, int, sc_uint<4 to 64>, sc_signal, sc_out, sc_in, 
#             sc_in_clk)
# 04052005  : check range function
# 07052005  : check range function and concat operator
# 16062005  : check all the functions to browse model hierarchy.
# 21062005  : check to_bool, to_string() functions
# 05092005  : trying to bind a signal to an unknown type
# 08092005  : unable to select the best method to schedule
# 09092005  : ~ operator
# 09092005b : starting clock edge
# 09092005c : 1 module with 1 sc_out connected to 3 inner IP. 
#             (2 sc_in and 2 sc_out)
# 14092005  : check vcd trace (sc_in, sc_out, sc_signal, bool, int)
# 15092005  : check port dependency declaration. 3 different error messages.
# 28102005  : check sc_time::to_string (), to_seconds (), to_double ().
# 16112005  : check "sc_out and sc_in not bound" messages.
# 16122005  : check sc_module_name casts.
# 19122005  : check simulator state saving for Socview compatibility
# 17022006  : check rename function for Socview compatibility
# 02052006  : check sc_uint, sc_int, sc_signal<X> when X is double, char, 
#             signed int, unsigned int and so on. (little/big endianness test)
# 07122006  : check if we can declare an undefined external function as 
#             a sc_method.
#             check a virtual method as a sc_method
#             (Unable to check if sc_method is a virtual one)
# 20122006  : check Petrot's scheduling (CASS, quasi static)
#                   Mouchard's scheduling (FastSysC, incomplete static)
#                   Buchmann's scheduling (SystemCASS, totally static)
# 16022007  : check --nobanner option
# 23032007  : check multiple execution on the same computer
# 15042009  : check sc_uint operators (==, =, <<=, +=)
#             in some cases, such as a compairison, the developper has to use
#             sc_signal<T>::read() method to explicitly read a sc_signal.

## Uncomment the following variable if you want to compile and run the 
## SOCVIEW regression test
##

#SOCVIEW_TEST=19122005 \
             17022006 \
#


ifndef SYSTEMC
$(error Please set your SYSTEMC environment variable on the systemc directory root)
endif
ifndef SYSTEMCASS
$(error Please set your SYSTEMCASS environment variable on the SystemCass directory root)
endif


DIR = \
      02052006  \
      04052005  \
      05092005  \
      07052005  \
      07122006a \
      07122006b \
      08092005  \
      09092005a \
      09092005b \
      09092005c \
		11062007  \
      14092005  \
      15042009a \
      15042009b \
      15042009c \
		15062006  \
      15092005a \
      15092005b \
      15092005c \
      15092005d \
		17022006  \
      17032005  \
      20122006  \
      21062005  \
		24082009  \
      25032005  \
      28102005  \
      29032005  \
      30032005a \
      30032005b \
      30032005c \
      16022007  \
      16062005a \
      16062005b \
      16112005a \
      16112005b \
      16112005c \
      16122005  \
      19042005 
      #$(SOCVIEW_TEST) \


all: compile

compile :
	@for i in $(DIR) ; do \
   ($(MAKE) --no-print-directory -C $$i) ; \
   done;

test : $(DIR)
	@for i in $(DIR) ; do \
   ($(MAKE) --no-print-directory -C $$i test | grep "Testing") ; \
   done;
	
clean :
	@for i in $(DIR) ; do \
   ($(MAKE) --no-print-directory -C $$i clean) ; \
   done;

