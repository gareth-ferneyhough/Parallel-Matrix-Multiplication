LIBS  = -lboost_system
CFLAGS = -g -Wall -O3 -L /usr/local/lib

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)

all:	 matrix par 

matrix:	matrix.cpp matrix.h
	mpic++ -c matrix.cpp $(CFLAGS) $(LIBS) -lboost_mpi

par:	matrix_multiplication_parallel.cpp
	mpic++ -o par matrix_multiplication_parallel.cpp matrix.o $(CFLAGS) $(LIBS) -lboost_mpi

clean:
	rm *.o par