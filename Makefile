CXX=g++
CC=g++
CXXFLAGS=-O2 -g -Wall
CFLAGS=-O2 -g -Wall
LDFLAGS=-O2 -g

all: solve_array solve_sudoku solve_andrew test_opengl

solve_array: solve_array.o array.o dancing.o

solve_sudoku: solve_sudoku.o dancing.o

solve_andrew: solve_andrew.o dancing.o andrew.o

test_opengl: test_opengl.o
	$(CC) $(LDFLAGS) -o test_opengl test_opengl.o -lGL -lglut

clean:
	$(RM) solve_andrew solve_sudoku solve_array test_opengl *.o

