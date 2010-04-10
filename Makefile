CXX=g++
CC=g++
CXXFLAGS=-g -Wall
LDFLAGS=-g -lefence

all: solve_array solve_sudoku solve_andrew

solve_array: solve_array.o array.o dancing.o

solve_sudoku: solve_sudoku.o dancing.o

solve_andrew: solve_andrew.o dancing.o

clean:
	$(RM) solve_sudoku solve_array *.o

