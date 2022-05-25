#
# FIT BUT, SFC, 1. project
# Pavol Plaskon, xplask00
#

CXX=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -pedantic -O2
TARGET=pso-tsp

SRC_FILES = $(wildcard *.cpp)
HEADER_FILES = $(wildcard *.hpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))


.PHONY: all clean pack

all: $(TARGET)


$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)

pack:
	cp doc/doc.pdf .
	zip xplask00 Makefile *.cpp *.hpp doc.pdf makeplot.sh plot_result.plg cities -r
	rm doc.pdf
