CC=gcc
CFLAGS=-O2 -Wall
OBJ=data.o hash_table.o nary_tree.o graph.o b_tree.o main.o
EXEC=comparacion

build: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)
