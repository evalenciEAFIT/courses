CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = backup_EAFITos
SRC = backup.c

# Regla por defecto (compila el programa principal)
all: $(TARGET)

# Cómo construir el ejecutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar los archivos generados y los de prueba
clean:
	rm -f $(TARGET)
	rm -rf src_test dest_test test_file.txt test_file_backup.txt

# Regla para ejecutar pruebas automáticas
test: $(TARGET)
	@echo "\n--- [1] PREPARANDO ENTORNO DE PRUEBA ---"
	mkdir -p src_test
	echo "Hola, este es un archivo de prueba 1" > src_test/archivo1.txt
	echo "Esto es otra prueba" > src_test/archivo2.txt
	mkdir -p src_test/subcarpeta
	echo "Archivo en subcarpeta" > src_test/subcarpeta/archivo3.txt
	echo "Archivo simple" > test_file.txt

	@echo "\n--- [2] PROBANDO BACKUP DE ARCHIVO SIMPLE ---"
	./$(TARGET) -b test_file.txt test_file_backup.txt

	@echo "\n--- [3] PROBANDO BACKUP DE DIRECTORIO ---"
	./$(TARGET) -b src_test dest_test

	@echo "\n--- [4] REVISANDO EL DIRECTORIO DE BACKUP ---"
	ls -R dest_test

.PHONY: all clean test
