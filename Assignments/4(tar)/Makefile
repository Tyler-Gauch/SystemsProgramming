SRC = ./src
BIN = ./bin
INC = ./inc

CCX = g++
CFLAGS = -Wall

default: tar.o
	$(CCX) $(CFLAGS) -o $(BIN)/tar $(BIN)/tar.o

tar.o: $(SRC)/tar.cpp
	$(CXX) $(CFLAGS) -o $(BIN)/tar.o -c $(SRC)/tar.cpp
