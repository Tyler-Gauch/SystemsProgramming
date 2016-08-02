SRC=./src
BIN=./bin
INC=./inc
CC=g++
CFLAGS=-Wall

main: main.o person.o
	$(CC) $(CFLAGS) -o $(BIN)/ftree $(BIN)/*.o

main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c -o $(BIN)/main.o $(SRC)/main.cpp

person.o: $(INC)/Person.h $(SRC)/Person.cpp
	$(CC) $(CFLAGS) -c -o $(BIN)/person.o $(SRC)/Person.cpp

clean:
	rm -rf $(BIN)/*