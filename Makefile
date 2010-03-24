CXX=g++
CC=g++
CXXFLAGS=-g -Wall
LDFLAGS=-lefence

all: main

main: main.o array.o dancing.o

clean:
	$(RM) main *.o

