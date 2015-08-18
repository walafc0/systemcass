

TARGET_ARCH = linux64

ifeq (${TARGET_ARCH},Darwin)
DYNAMICLIB_FLAG = -dynamic
ENDIANESS_FLAG  = #-mlittle-endian
else
DYNAMICLIB_FLAG = -rdynamic
ENDIANESS_FLAG  = 
endif
ifndef SYSTEMC
$(error SYSTEMC should be set to SystemC directory (OSCI version) \
to compare simulation results.)
endif

SYSTEMC_INC     = $(SYSTEMC)/include
SYSTEMCASS_INC  = $(SYSTEMCASS)/include
SYSTEMC_LIB     = $(SYSTEMC)/lib-${TARGET_ARCH}/libsystemc.a
SYSTEMCASS_LIB  = $(SYSTEMCASS)/lib-linux/libsystemc.a
CXX             = g++
CPPFILT         = c++filt
DEBUG           = -g -DDEBUG
#OPT             = -O2
#PROF            = -pg # gcc
#PROF            = -qp # icc
#EFENCE          = /users/cao/viaud/bin/lib/libefence.a -lpthread
CFLAGS = $(PROF) $(DEBUG) $(OPT) \
              -Wall \
              -fopenmp \
              -UENABLE_TRACE -UENABLE_PAT -I. -I.. \
              ${ENDIANESS_FLAG}

LFLAGS = $(PROF) $(BFD_LIBERTY) -ldl -fopenmp

CFLAGS_SYSTEMC    = $(CFLAGS) \
                    -I$(SYSTEMC_INC) \

CFLAGS_SYSTEMCASS = $(CFLAGS) \
                    -I$(SYSTEMCASS_INC) \
                    ${DYNAMICLIB_FLAG} #-Wno-deprecated

LFLAGS_SYSTEMC    = $(LFLAGS) \
                    $(SYSTEMC_LIB)

LFLAGS_SYSTEMCASS = $(LFLAGS) \
                    ${DYNAMICLIB_FLAG} $(SYSTEMCASS_LIB) ${EFENCE}

ECHO = /bin/echo
SILENCE = @

