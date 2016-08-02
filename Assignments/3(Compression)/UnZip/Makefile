CXX = g++
CFLAGS = -g -Wall

ifndef MASTER_UNZIP
MASTER_UNZIP = .
endif

ifndef OUT
OUT = ..
endif

SRC = $(MASTER_UNZIP)/src
BIN = $(MASTER_UNZIP)/bin
INC = $(MASTER_UNZIP)/inc

unzip_main: unzip_main.o unzip.o
	$(CXX) $(CFLAGS) -o $(OUT)/unzip $(BIN)/unzip.o $(BIN)/main.o

unzip_main.o: unzip.o $(SRC)/main.cpp
	$(CXX) $(CFLAGS) -o $(BIN)/main.o -c $(SRC)/main.cpp

unzip.o:
	$(CXX) $(CFLAGS) -o $(BIN)/unzip.o -c $(SRC)/unzip.cpp

unzip_clean: 
	rm -rf $(BIN)/*