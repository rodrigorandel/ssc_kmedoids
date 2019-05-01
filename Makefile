CC = g++

#CPLEX_VERSION = CPLEX_Studio128
#
#CPLEX_LIB = x86-64_osx
#
#CPLEX_DIR = /Applications/$(CPLEX_VERSION)/cplex
#CPLEX_LIB_DIR = $(CPLEX_DIR)/lib/$(CPLEX_LIB)/static_pic
#CPLEX_INC_DIR = $(CPLEX_DIR)/include
#
#CONCERT_DIR = /Applications/$(CPLEX_VERSION)/concert
#CONCERT_LIB_DIR = $(CONCERT_DIR)/lib/$(CPLEX_LIB)/static_pic
#CONCERT_INC_DIR = $(CONCERT_DIR)/include
#
#CCOPT = -std=c++11 -g -fPIC -fexceptions -DNDEBUG -DIL_STD -Wno-deprecated
#CCLNFLAGS = -L$(CPLEX_LIB_DIR) -lilocplex -lcplex -L$(CONCERT_LIB_DIR) -lconcert -m64 -lm -lpthread
#CCFLAGS = $(CCOPT) -I$(CPLEX_INC_DIR) -I$(CONCERT_INC_DIR)

CCFLAGS = -O3 -std=c++11 
# -I/pathtoboostofbrew

#LIBS = -L$(CPLEX_LIB_DIR) -lilocplex -lcplex -L$(CONCERT_LIB_DIR) -lconcert -lm -lpthread

OBJS = bibrand2.o Point.o  Instance.o Extra.o Solution.o Vns.o Constraints.o Main.o


TARGET = main

%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CCFLAGS) -c -o $@ $<

#-L/usr/local/Cellar/boost/1.69.0/lib/ -lboost_serialization
all: $(OBJS)
	$(CC) $(CCFLAGS) $^ $(CCLNFLAGS) -o $(TARGET) 

exec: Exec.cpp
	$(CC) -std=c++11 -O2 -w -pthread -fopenmp -o exec Exec.cpp

exec2: ExecGraphics.cpp
	$(CC) -std=c++11 -g -w -o exec2 ExecGraphics.cpp

execConfusion: ExecConfusion.cpp
	$(CC) -std=c++11 -g -w -o execConfusion ExecConfusion.cpp

distances: Point.o CreateDistanceMatrix.cpp
	$(CC) -std=c++11 -fopenmp -o distance CreateDistanceMatrix.cpp Point.o

clean:
	rm $(OBJS) $(OBJS_CPLEX) $(TARGET) exec distance
