CXX = g++ -std=c++11
CHECKSTYLE = python ../cpplint.py
MAIN = $(basename $(wildcard *Main.cpp))
TEST = $(basename $(wildcard *Test.cpp))
OBJECTS = $(addsuffix .o, $(filter-out %Main %Test, $(basename $(wildcard *.cpp))))
HEADERS = $(wildcard *.h)
LIB_PATH = /home/felix/gurobi752/linux64/lib
LIBS = -lgurobi_c++ -lgurobi75 -lboost_filesystem -lboost_system
CFLAGS = /home/felix/gurobi752/linux64/include

.PRECIOUS: %.o

all: compile test checkstyle

compile: $(MAIN)

test: $(TEST)
	for T in $(TEST); do ./$$T; done

checkstyle:
	$(CHECKSTYLE) *.cpp *.h

%Main: %Main.o $(OBJECTS) 
	$(CXX) -o $@ $^ -L${LIB_PATH} ${LIBS} 

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ -L${LIB_PATH} ${LIBS} -lgtest -lgtest_main -lpthread 

%.o: %.cpp $(HEADERS) 
	$(CXX) -I${CFLAGS} -c $<

clean:
	rm -f *.o *.log
	rm -f $(MAIN)
	rm -f $(TEST)
