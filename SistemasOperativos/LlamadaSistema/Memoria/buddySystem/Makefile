# Definir el compilador
CC = g++

# Bandera de compilación
CFLAGS = -Wall -std=c++17

# Nombre del ejecutable
TARGET = programa_buddy

# Archivos fuente
SRCS = main.cpp imagen.cpp buddy_allocator.cpp stb_wrapper.cpp
# Archivos objeto generados
OBJS = $(SRCS:.cpp=.o)

# Regla para compilar el programa
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Regla para compilar cada archivo fuente a objeto
%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos objeto y ejecutables
clean:
	rm -f $(OBJS) $(TARGET)

# Ejecutar el programa
run:
	./$(TARGET) ./test/testImg01.jpg xxx.jpg -buddy
