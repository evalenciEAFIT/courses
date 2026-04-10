# ================================================================
#  Makefile — Proyecto COMPRIMIR (EDA Pedagógico)
#  Algoritmos: RLE · Huffman · LZ77
#
#  Estructura:
#    src/   → código fuente (.c / .h)
#    bin/   → objetos (.o) + ejecutable
#    app/   → script lanzador (wrapper)
#    doc/   → documentación
#    samples/fuente      → archivos de entrada
#    samples/comprimidos → salidas comprimidas
#    samples/descomprimidos → salidas descomprimidas
# ================================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -O2 \
          -D_POSIX_C_SOURCE=200809L \
          -DSAMPLES_DIR=\"samples\"

SRC_DIR = src
BIN_DIR = bin
APP_DIR = app
TARGET  = $(BIN_DIR)/comprimir

SRCS    = $(SRC_DIR)/main.c     \
          $(SRC_DIR)/utils.c    \
          $(SRC_DIR)/rle.c      \
          $(SRC_DIR)/huffman.c  \
          $(SRC_DIR)/lz.c

OBJS    = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

DEPS    = $(SRC_DIR)/utils.h    \
          $(SRC_DIR)/rle.h      \
          $(SRC_DIR)/huffman.h  \
          $(SRC_DIR)/lz.h

# ── Directorios necesarios ────────────────────────────────────
DIRS = $(BIN_DIR) $(APP_DIR) \
       samples/fuente samples/comprimidos samples/descomprimidos \
       doc

# ── Regla principal ───────────────────────────────────────────
.PHONY: all run debug valgrind clean help dirs

all: dirs $(TARGET) $(APP_DIR)/comprimir

dirs:
	@mkdir -p $(DIRS)

# Enlazado final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lrt
	@echo ""
	@echo "  ✔  Ejecutable : $(TARGET)"
	@echo "  ────────────────────────────────────────────────"

# Compilación de cada módulo: src/*.c → bin/*.o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Wrapper en app/ — script que llama al binario desde cualquier dir
$(APP_DIR)/comprimir: $(TARGET)
	@echo '#!/usr/bin/env bash' > $@
	@echo 'cd "$$(dirname "$$0")/.." && exec ./$(TARGET) "$$@"' >> $@
	@chmod +x $@
	@echo "  ✔  Lanzador  : $(APP_DIR)/comprimir"

# ── Atajos ────────────────────────────────────────────────────
run: all
	@./$(TARGET)

debug: CFLAGS += -g -DDEBUG -fsanitize=address,undefined
debug: clean all

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

clean:
	@rm -f $(BIN_DIR)/*.o $(TARGET) $(APP_DIR)/comprimir
	@rm -f samples/comprimidos/* samples/descomprimidos/*
	@echo "  ✔  Limpieza completa."

help:
	@echo ""
	@echo "  ┌─ Comandos disponibles ──────────────────────────┐"
	@echo "  │  make          → compilar todo                  │"
	@echo "  │  make run      → compilar y ejecutar            │"
	@echo "  │  make debug    → con AddressSanitizer           │"
	@echo "  │  make valgrind → análisis de memoria            │"
	@echo "  │  make clean    → eliminar binarios y salidas     │"
	@echo "  └─────────────────────────────────────────────────┘"
	@echo ""
