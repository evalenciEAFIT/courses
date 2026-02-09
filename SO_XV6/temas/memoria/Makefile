# Makefile para el proyecto de Memoria Virtual
# Proyecto educativo sobre gestión de memoria en Linux

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

# Archivos
TARGET = mem1
SOURCES = mem1.c
OBJECTS = $(SOURCES:.c=.o)

# Colores para output (opcional)
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Regla por defecto
all: $(TARGET)
	@echo "$(GREEN)✓ Compilación exitosa!$(NC)"
	@echo "$(YELLOW)Ejecuta 'make run' para correr el programa$(NC)"

# Compilar el programa principal
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Enlazando $(TARGET)...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Compilar archivos objeto
%.o: %.c
	@echo "$(YELLOW)Compilando $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar el programa
run: $(TARGET)
	@echo "$(GREEN)Ejecutando $(TARGET)...$(NC)"
	@echo "$(YELLOW)================================$(NC)"
	./$(TARGET)

# Ejecutar con valgrind para detectar memory leaks
valgrind: $(TARGET)
	@echo "$(YELLOW)Ejecutando con valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Ver el mapa de memoria del proceso
memmap: $(TARGET)
	@echo "$(YELLOW)Ejecutando programa en background...$(NC)"
	@./$(TARGET) & echo $$! > .pid
	@sleep 1
	@echo "$(GREEN)Mapa de memoria del proceso:$(NC)"
	@cat /proc/$$(cat .pid)/maps || true
	@kill $$(cat .pid) 2>/dev/null || true
	@rm -f .pid

# Limpiar archivos compilados
clean:
	@echo "$(RED)Limpiando archivos compilados...$(NC)"
	rm -f $(TARGET) $(OBJECTS) .pid
	@echo "$(GREEN)✓ Limpieza completa$(NC)"

# Mostrar información del sistema
info:
	@echo "$(GREEN)=== Información del Sistema ===$(NC)"
	@echo "Tamaño de página: $$(getconf PAGE_SIZE) bytes"
	@echo "Memoria total: $$(free -h | awk '/^Mem:/ {print $$2}')"
	@echo "Compilador: $$($(CC) --version | head -n1)"
	@echo "Kernel: $$(uname -r)"

# Ayuda
help:
	@echo "$(GREEN)=== Makefile - Proyecto Memoria Virtual ===$(NC)"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  $(YELLOW)make$(NC)          - Compila el programa"
	@echo "  $(YELLOW)make run$(NC)      - Compila y ejecuta el programa"
	@echo "  $(YELLOW)make valgrind$(NC) - Ejecuta con valgrind (detecta memory leaks)"
	@echo "  $(YELLOW)make memmap$(NC)   - Muestra el mapa de memoria del proceso"
	@echo "  $(YELLOW)make clean$(NC)    - Elimina archivos compilados"
	@echo "  $(YELLOW)make info$(NC)     - Muestra información del sistema"
	@echo "  $(YELLOW)make help$(NC)     - Muestra esta ayuda"
	@echo ""
	@echo "Ejemplos:"
	@echo "  $$ make clean && make run"
	@echo "  $$ make valgrind"

# Declarar targets que no son archivos
.PHONY: all run valgrind memmap clean info help
