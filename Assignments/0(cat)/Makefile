SRC=./src
BIN=./bin
CC=g++
CFLAGS=-Wall

cat: cat.o
	$(CC) $(CFLAGS) -o $(BIN)/cat $(BIN)/cat.o

cat.o:
	$(CC) $(CFLAGS) -c -o $(BIN)/cat.o $(SRC)/cat.cpp

clean:
	rm -rf $(BIN)/*