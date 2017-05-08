CXX = g++
SOURCES = ./pfspinstance.cpp ./solution.cpp ./localsearch.cpp ./tabu.cpp ./ILS.cpp
CPPFLAGS = -Wall -O3
CXXFLAGS := -std=c++11
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o)

clean: 
	rm ./*.o

