include ../env.mk

SOCVIEW_INC    = ${SOCVIEW}/include
SOCVIEW_LIB    = ${SOCVIEW}/lib-linux/libsystemc.a
CFLAGS_SOCVIEW = ${CFLAGS} -I${SOCVIEW_INC}
LFLAGS_SOCVIEW = ${LFLAGS} -ltermcap -lreadline -lpthread ${SOCVIEW_LIB}

SYSTEM    = system.cpp system2.cpp
EXE_SCASS = $(SYSTEM:.cpp=_systemcass.x)
EXE_SC    = $(SYSTEM:.cpp=_systemc.x)
EXE_SVIEW = $(SYSTEM:.cpp=_socview.x)
EXE       = ${EXE_SCASS} ${EXE_SVIEW} ${EXE_SC}
OBJECTS   = $(EXE:.x=.o)
LINKS     = $(OBJECTS:.o=.cpp)

TEST1_DAT = \
           test_socview_1.dat test_socview_1_reloaded.dat   \
           test_socview_2.dat test_socview_2_reloaded.dat   \
           test_socview_3.dat test_socview_3_reloaded.dat   \
           test_socview_3b.dat test_socview_3_reloaded_bis.dat   \
           test_socview_4.dat test_socview_4_reloaded.dat   \
           test_socview_5.dat test_socview_5_reloaded.dat   \
           test_socview_6.dat test_socview_6_reloaded.dat   \
           test_socview_7.dat test_socview_7_reloaded.dat   \
           test_socview_8.dat test_socview_8_reloaded.dat   \
           test_socview_9.dat test_socview_9_reloaded.dat   \
           test_socview_10.dat test_socview_10_reloaded.dat \
           test_socview_10b.dat \
           test_systemcass_0.dat \
           test_systemcass_1.dat \
           test_systemcass_2.dat \
           test_systemcass_3.dat \
           test_systemcass_4.dat \
           test_systemcass_5.dat \
           test_systemcass_6.dat \
           test_systemcass_7.dat \
           test_systemcass_8.dat \
           test_systemcass_9.dat \
           test_systemcass_10.dat \
           test_systemcass_11.dat \
#
TEST2_DAT = test2_socview_10.dat \
            test2_socview_24a.dat test2_socview_24b.dat test2_socview_24c.dat \
            test2_socview_20a.dat test2_socview_20b.dat test2_systemcass_20.dat
#
TEST3 = 20
TEST3_DAT = \
            $(foreach i,${TEST3},test3_systemcass_$(i).dat)
#
.SECONDARY:

main : $(EXE)

test : test1 test2 test3

test1 : ${TEST1_DAT}
	diff test_socview_10.dat test_socview_10b.dat
	diff test_socview_1.dat test_socview_1_reloaded.dat
	diff test_socview_2.dat test_socview_2_reloaded.dat
	diff test_socview_3.dat test_socview_3b.dat
	diff test_socview_3.dat test_socview_3_reloaded_bis.dat
	diff test_socview_3.dat test_socview_3_reloaded.dat
	diff test_socview_4.dat test_socview_4_reloaded.dat
	diff test_socview_5.dat test_socview_5_reloaded.dat
	diff test_socview_6.dat test_socview_6_reloaded.dat
	diff test_socview_7.dat test_socview_7_reloaded.dat
	diff test_socview_8.dat test_socview_8_reloaded.dat
	diff test_socview_9.dat test_socview_9_reloaded.dat
	diff test_socview_10.dat test_socview_10_reloaded.dat
	@echo Test OK.

test2 : ${TEST2_DAT}
	diff test2_socview_20a.dat test2_socview_20b.dat
	diff test2_socview_24a.dat test2_socview_24c.dat
	diff test2_socview_24a.dat test2_socview_24b.dat
	
test3 : ${TEST3_DAT} ${TEST2_DAT}
	for i in ${TEST3} ; do \
    diff test2_systemcass_$${i}.dat test3_systemcass_$${i}.dat \
      || eval ${failcom} ; \
  done

${TEST1_DAT} : system_socview.x system_systemcass.x socview_check_command socview_command
	rm -f ${TEST1_DAT}
	./system_socview.x 11 < socview_command
	./system_systemcass.x 11
	./system_socview.x 11 < socview_check_command

${TEST2_DAT} : system2_systemcass.x system2_socview.x socview_command2
	rm -f ${TEST2_DAT}
	./system2_systemcass.x 20
	./system2_socview.x 20 < socview_command2

${TEST3_DAT} : system3_systemcass.x
	for i in ${TEST3} ; do \
    system3_systemcass.x --save_on_exit test3_systemcass_$${i}.dat $${i} \
      || eval ${failcom} ; \
  done

%.gif : %.dot
	dot -Tgif -o $*.gif $*.dot

%_systemc.x : %_systemc.o $(SYSTEMC_LIB)
	$(CXX) -o $@ $*_systemc.o $(LFLAGS_SYSTEMC)

%_systemcass.x : %_systemcass.o  $(SYSTEMCASS_LIB)
	$(CXX) -o $@ $*_systemcass.o $(LFLAGS_SYSTEMCASS)

%_socview.x : %_socview.o  $(SOCVIEW_LIB)
	$(CXX) -o $@ $*_socview.o $(LFLAGS_SOCVIEW)

-include Makefile.deps

%_systemc.cpp : %.cpp 
	ln -s $*.cpp $*_systemc.cpp

%_systemcass.cpp : %.cpp 
	ln -s $*.cpp $*_systemcass.cpp

%_socview.cpp : %.cpp 
	ln -s $*.cpp $*_socview.cpp

%_systemc.o : %_systemc.cpp 
	$(CXX) $(CFLAGS_SYSTEMC) -MM $*_systemc.cpp >> Makefile.deps
	$(CXX) $(CFLAGS_SYSTEMC) -c $*_systemc.cpp -o $*_systemc.o

%_systemcass.o : %_systemcass.cpp
	$(CXX) $(CFLAGS_SYSTEMCASS) -MM $*_systemcass.cpp >> Makefile.deps
	$(CXX) $(CFLAGS_SYSTEMCASS) -c $*_systemcass.cpp -o $*_systemcass.o

%_socview.o : %_socview.cpp
	$(CXX) $(CFLAGS_SOCVIEW) -MM $*_socview.cpp >> Makefile.deps
	$(CXX) $(CFLAGS_SOCVIEW) -c $*_socview.cpp -o $*_socview.o

cleandat :
	-rm -f ${TEST1_DAT} ${TEST2_DAT} ${TEST3_DAT}

clean : cleandat
	rm -f Makefile.deps
	-rm -f *.o gmon.out *~
	-rm -f results
	-rm -f signal_order.txt module_order.dot signal_graph.dot
	-rm -f module_order.gif signal_graph.gif
	-rm -f $(EXE) $(OBJECTS) 
	-for i in $(LINKS) ; do unlink $$i ; done 2> /dev/null
	-rm -f core*
	-rm -rf generated_by_systemcass
	-rm -rf system_systemcass.x.vcd system_systemc.x.vcd
	
