CXX = g++
CXXFLAGS = -Wall -g

mytest: dtree.o utree.o mytest.cpp
	$(CXX) $(CXXFLAGS) dtree.o utree.o mytest.cpp -o mytest

dtree.o: dtree.h dtree.cpp
	$(CXX) $(CXXFLAGS) -c dtree.cpp

utree.o: utree.h utree.cpp
	$(CXX) $(CXXFLAGS) -c utree.cpp

run: 
	./mytest

gdb:
	gdb ./mytest

val:
	valgrind --leak-check=full -s  --track-origins=yes ./mytest
