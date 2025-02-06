CC = g++
CFLAGS = -Wall -Wextra -std=c++17

all: UsoBits

UsoBits: main.o ManejadorArchivo.o
	$(CC) $(CFLAGS) -o UsoBits main.o ManejadorArchivo.o

main.o: main.cpp ManejadorArchivo.h
	$(CC) $(CFLAGS) -c main.cpp

ManejadorArchivo.o: ManejadorArchivo.cpp ManejadorArchivo.h
	$(CC) $(CFLAGS) -c ManejadorArchivo.cpp

clean:
	rm -f UsoBits *.o
