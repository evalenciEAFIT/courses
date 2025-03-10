CC = g++
CFLAGS = -Wall

all: main

main: main.o Comparador.o
	$(CC) $(CFLAGS) main.o Comparador.o -o programa -lm

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Comparador.o: Comparador.cpp Comparador.h
	$(CC) $(CFLAGS) -c Comparador.cpp

clean:
	rm -f *.o programa
