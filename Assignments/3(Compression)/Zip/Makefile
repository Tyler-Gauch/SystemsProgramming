CXX = g++
CFLAGS = -g -Wall

ifndef MASTER_ZIP
MASTER_ZIP = .
endif

ifndef OUT
OUT = ..
endif

SRC = $(MASTER_ZIP)/src
BIN = $(MASTER_ZIP)/bin
INC = $(MASTER_ZIP)/inc

zip_main: zip_main.o zip.o
	$(CXX) $(CFLAGS) -o $(OUT)/zip $(BIN)/zip.o $(BIN)/main.o

zip_main.o: zip.o $(SRC)/main.cpp
	$(CXX) $(CFLAGS) -o $(BIN)/main.o -c $(SRC)/main.cpp

zip.o:
	$(CXX) $(CFLAGS) -o $(BIN)/zip.o -c $(SRC)/zip.cpp

zip_clean: 
	rm -rf $(BIN)/*