CXX	= /usr/local/gcc-7.1/bin/g++-7.2
CC      = /usr/local/gcc-7.1/bin/gcc-7.2
RM=rm -f
LDFLAGS= 
LDLIBS=

OBJS = WindSimTest.o WindSim.o windsim_dk1_main.o windsim_dk1_main_emxAPI.o windsim_dk1_main_emxutil.o rtGetInf.o rtGetNaN.o rt_nonfinite.o

WindSim:$(OBJS)
	$(CXX) $(LDFLAGS) -o WindSimTest $(OBJS) $(LDLIBS)

WindSimTest.o:WindSimTest.cpp

WindSim.o:WindSim.h

windsim_dk1_main.o:windsim_dk1_main.h

windsim_dk1_main_emxAPI.o:windsim_dk1_main_emxAPI.h

windsim_dk1_main_emxutil.o:windsim_dk1_main_emxutil.h

clean:
	rm -f WindSimTest $(OBJS) *.o *~
