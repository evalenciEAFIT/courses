# Makefile para las Soluciones de Ejercicios de Memoria Virtual
# Proyecto educativo sobre gestión de memoria en Linux

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lm

# Archivos fuente
SRC_EJ1 = ejercicio1_solucion.c
SRC_EJ2 = ejercicio2_shared_memory_solucion.c
SRC_EJ3 = ejercicio3_mmap_file_solucion.c
SRC_EJ4 = ejercicio4_page_faults_solucion.c
SRC_EJ5 = ejercicio5_cow_solucion.c

# Ejecutables
TARGET_EJ1_BASICA = ejercicio1_basica
TARGET_EJ1_INTERMEDIA = ejercicio1_intermedia
TARGET_EJ1_AVANZADA = ejercicio1_avanzada
TARGET_EJ2 = ejercicio2_sol
TARGET_EJ3 = ejercicio3_sol
TARGET_EJ4 = ejercicio4_sol
TARGET_EJ5 = ejercicio5_sol

# Colores para output
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RED = \033[0;31m
MAGENTA = \033[0;35m
NC = \033[0m

# Regla por defecto: compilar todos
all: ej1-avanzada ej2 ej3 ej4 ej5
	@echo "$(GREEN)✓ Todos los ejercicios compilados$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJERCICIO 1: Allocator Simple
# ═══════════════════════════════════════════════════════════════

ej1-basica:
	@echo "$(YELLOW)Compilando Ejercicio 1 (versión BÁSICA)...$(NC)"
	$(CC) $(CFLAGS) -DVERSION_BASICA -o $(TARGET_EJ1_BASICA) $(SRC_EJ1)
	@echo "$(GREEN)✓ $(TARGET_EJ1_BASICA) compilado$(NC)"

ej1-intermedia:
	@echo "$(YELLOW)Compilando Ejercicio 1 (versión INTERMEDIA)...$(NC)"
	$(CC) $(CFLAGS) -DVERSION_INTERMEDIA -o $(TARGET_EJ1_INTERMEDIA) $(SRC_EJ1)
	@echo "$(GREEN)✓ $(TARGET_EJ1_INTERMEDIA) compilado$(NC)"

ej1-avanzada:
	@echo "$(YELLOW)Compilando Ejercicio 1 (versión AVANZADA)...$(NC)"
	$(CC) $(CFLAGS) -DVERSION_AVANZADA -o $(TARGET_EJ1_AVANZADA) $(SRC_EJ1)
	@echo "$(GREEN)✓ $(TARGET_EJ1_AVANZADA) compilado$(NC)"

ej1-todas: ej1-basica ej1-intermedia ej1-avanzada
	@echo "$(GREEN)✓ Todas las versiones del Ejercicio 1 compiladas$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJERCICIO 2: Memoria Compartida
# ═══════════════════════════════════════════════════════════════

ej2:
	@echo "$(YELLOW)Compilando Ejercicio 2 (Memoria Compartida)...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET_EJ2) $(SRC_EJ2)
	@echo "$(GREEN)✓ $(TARGET_EJ2) compilado$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJERCICIO 3: Memory-Mapped Files
# ═══════════════════════════════════════════════════════════════

ej3:
	@echo "$(YELLOW)Compilando Ejercicio 3 (Memory-Mapped Files)...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET_EJ3) $(SRC_EJ3)
	@echo "$(GREEN)✓ $(TARGET_EJ3) compilado$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJERCICIO 4: Page Faults
# ═══════════════════════════════════════════════════════════════

ej4:
	@echo "$(YELLOW)Compilando Ejercicio 4 (Page Faults)...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET_EJ4) $(SRC_EJ4) $(LDFLAGS)
	@echo "$(GREEN)✓ $(TARGET_EJ4) compilado$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJERCICIO 5: Copy-on-Write
# ═══════════════════════════════════════════════════════════════

ej5:
	@echo "$(YELLOW)Compilando Ejercicio 5 (Copy-on-Write)...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET_EJ5) $(SRC_EJ5)
	@echo "$(GREEN)✓ $(TARGET_EJ5) compilado$(NC)"

# ═══════════════════════════════════════════════════════════════
# EJECUCIÓN
# ═══════════════════════════════════════════════════════════════

run-ej1: ej1-avanzada
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@echo "$(BLUE)  Ejecutando Ejercicio 1: Allocator Simple$(NC)"
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@./$(TARGET_EJ1_AVANZADA)

run-ej2: ej2
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@echo "$(BLUE)  Ejecutando Ejercicio 2: Memoria Compartida$(NC)"
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@./$(TARGET_EJ2)

run-ej3: ej3
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@echo "$(BLUE)  Ejecutando Ejercicio 3: Memory-Mapped Files$(NC)"
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@./$(TARGET_EJ3)

run-ej4: ej4
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@echo "$(BLUE)  Ejecutando Ejercicio 4: Page Faults$(NC)"
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@./$(TARGET_EJ4)

run-ej5: ej5
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@echo "$(BLUE)  Ejecutando Ejercicio 5: Copy-on-Write$(NC)"
	@echo "$(BLUE)═══════════════════════════════════════════════════════════════$(NC)"
	@./$(TARGET_EJ5)

run-all: all
	@echo "$(MAGENTA)╔════════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(MAGENTA)║  Ejecutando TODOS los ejercicios                              ║$(NC)"
	@echo "$(MAGENTA)╚════════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@make run-ej1
	@echo "\n$(YELLOW)Presiona Enter para continuar...$(NC)" && read dummy
	@make run-ej2
	@echo "\n$(YELLOW)Presiona Enter para continuar...$(NC)" && read dummy
	@make run-ej3
	@echo "\n$(YELLOW)Presiona Enter para continuar...$(NC)" && read dummy
	@make run-ej4
	@echo "\n$(YELLOW)Presiona Enter para continuar...$(NC)" && read dummy
	@make run-ej5

# ═══════════════════════════════════════════════════════════════
# VALGRIND
# ═══════════════════════════════════════════════════════════════

valgrind-ej1: ej1-avanzada
	@echo "$(YELLOW)Ejecutando Ejercicio 1 con valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET_EJ1_AVANZADA)

valgrind-ej2: ej2
	@echo "$(YELLOW)Ejecutando Ejercicio 2 con valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET_EJ2)

valgrind-ej3: ej3
	@echo "$(YELLOW)Ejecutando Ejercicio 3 con valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET_EJ3)

# ═══════════════════════════════════════════════════════════════
# LIMPIEZA
# ═══════════════════════════════════════════════════════════════

clean:
	@echo "$(RED)Limpiando archivos compilados...$(NC)"
	rm -f $(TARGET_EJ1_BASICA) $(TARGET_EJ1_INTERMEDIA) $(TARGET_EJ1_AVANZADA)
	rm -f $(TARGET_EJ2) $(TARGET_EJ3) $(TARGET_EJ4) $(TARGET_EJ5)
	rm -f *.o core vgcore.* test_data.txt large_test.dat records.dat output.txt
	@echo "$(GREEN)✓ Limpieza completa$(NC)"

# ═══════════════════════════════════════════════════════════════
# ESTADÍSTICAS
# ═══════════════════════════════════════════════════════════════

stats:
	@echo "$(BLUE)╔════════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║  Estadísticas de las Soluciones                               ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(YELLOW)Líneas de código por ejercicio:$(NC)"
	@wc -l $(SRC_EJ1) $(SRC_EJ2) $(SRC_EJ3) $(SRC_EJ4) $(SRC_EJ5)
	@echo ""
	@echo "$(YELLOW)Total de archivos:$(NC)"
	@ls -1 *.c *.md Makefile 2>/dev/null | wc -l
	@echo ""
	@echo "$(YELLOW)Tamaño total:$(NC)"
	@du -sh . 2>/dev/null

# ═══════════════════════════════════════════════════════════════
# AYUDA
# ═══════════════════════════════════════════════════════════════

help:
	@echo "$(GREEN)╔════════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(GREEN)║  Makefile - Soluciones de Ejercicios de Memoria Virtual       ║$(NC)"
	@echo "$(GREEN)╚════════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(YELLOW)Compilación Individual:$(NC)"
	@echo "  make ej1-basica      - Ejercicio 1 (versión básica)"
	@echo "  make ej1-intermedia  - Ejercicio 1 (versión intermedia)"
	@echo "  make ej1-avanzada    - Ejercicio 1 (versión avanzada)"
	@echo "  make ej2             - Ejercicio 2 (Memoria Compartida)"
	@echo "  make ej3             - Ejercicio 3 (Memory-Mapped Files)"
	@echo "  make ej4             - Ejercicio 4 (Page Faults)"
	@echo "  make ej5             - Ejercicio 5 (Copy-on-Write)"
	@echo ""
	@echo "$(YELLOW)Compilación Grupal:$(NC)"
	@echo "  make all             - Compilar todos los ejercicios"
	@echo "  make ej1-todas       - Compilar todas las versiones del Ej1"
	@echo ""
	@echo "$(YELLOW)Ejecución:$(NC)"
	@echo "  make run-ej1         - Ejecutar Ejercicio 1"
	@echo "  make run-ej2         - Ejecutar Ejercicio 2"
	@echo "  make run-ej3         - Ejecutar Ejercicio 3"
	@echo "  make run-ej4         - Ejecutar Ejercicio 4"
	@echo "  make run-ej5         - Ejecutar Ejercicio 5"
	@echo "  make run-all         - Ejecutar TODOS los ejercicios"
	@echo ""
	@echo "$(YELLOW)Debugging:$(NC)"
	@echo "  make valgrind-ej1    - Ejecutar Ej1 con valgrind"
	@echo "  make valgrind-ej2    - Ejecutar Ej2 con valgrind"
	@echo "  make valgrind-ej3    - Ejecutar Ej3 con valgrind"
	@echo ""
	@echo "$(YELLOW)Utilidades:$(NC)"
	@echo "  make stats           - Mostrar estadísticas"
	@echo "  make clean           - Limpiar archivos compilados"
	@echo "  make help            - Mostrar esta ayuda"
	@echo ""
	@echo "$(BLUE)Ejemplos:$(NC)"
	@echo "  $$ make clean && make all"
	@echo "  $$ make run-all"
	@echo "  $$ make ej1-todas && make run-ej1"

.PHONY: all ej1-basica ej1-intermedia ej1-avanzada ej1-todas ej2 ej3 ej4 ej5 \
        run-ej1 run-ej2 run-ej3 run-ej4 run-ej5 run-all \
        valgrind-ej1 valgrind-ej2 valgrind-ej3 \
        stats clean help
