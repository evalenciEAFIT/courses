CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = encriptar
OBJS = main.o ciphers.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ciphers.h
	$(CC) $(CFLAGS) -c main.c

ciphers.o: ciphers.c ciphers.h
	$(CC) $(CFLAGS) -c ciphers.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) test.txt *.enc *.dec

.PHONY: all clean run

