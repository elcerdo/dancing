CXX=g++
CC=g++
CXXFLAGS=-g -Wall
LDFLAGS=-g -lefence

all: solve_array

solve_array: solve_array.o array.o dancing.o

clean:
	$(RM) main *.o

