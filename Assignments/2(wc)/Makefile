SRC=./src
BIN=./bin
INC=./inc
CC=g++
CFLAGS=-Wall

main: wc.o
	$(CC) $(CFLAGS) -o $(BIN)/wc $(BIN)/*.o

wc.o: $(SRC)/wc.cpp
	$(CC) $(CFLAGS) -o $(BIN)/wc.o -c $(SRC)/wc.cpp

clean:
	rm -rf $(BIN)/*