# Compilador y opciones
CC = gcc
CFLAGS = -Wall -g

# Archivos fuente y binarios
SRCS = $(wildcard *.c)
BINS = $(SRCS:.c=)

# Objetivo principal: Compilar todo
all: $(BINS)

# Regla para compilar cada binario individual
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# REGLA MÁGICA: Permite ejecutar cualquier programa con "make run-nombre"
# Ejemplo: make run-01_hola_archivo
run-%: %
	@echo "--- Ejecutando $< ---"
	@./$<
	@echo "--- Finalizado ---"

# Listar programas disponibles de forma amigable
list:
	@echo "Programas de File Management disponibles:"
	@ls *.c | sed 's/\.c//' | xargs -n 1 echo " -"

# Limpiar archivos de prueba y binarios
clean:
	rm -f $(BINS) *.txt *.bin
	@echo "Limpieza completada: Archivos generados eliminados."

# Ayuda
help:
	@echo "Opciones disponibles:"
	@echo "  make         - Compila todos los programas .c"
	@echo "  make <prog>  - Compila solo el programa indicado (ej. make 01_hola_archivo)"
	@echo "  make clean   - Borra binarios y archivos generados"
