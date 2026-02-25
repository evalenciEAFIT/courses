CC = gcc
CFLAGS = -Wall -O0 # O0 para que el compilador no nos optimice el ciclo 'for' que consume CPU
LDFLAGS = -pthread

all: procesos hilos

procesos: procesos.c
	$(CC) $(CFLAGS) -o procesos procesos.c

hilos: hilos.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o hilos hilos.c

clean:
	rm -f procesos hilos
