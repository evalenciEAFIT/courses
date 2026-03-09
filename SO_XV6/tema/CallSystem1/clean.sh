#!/bin/bash

echo "Limpiando archivos compilados, objetos y archivos de texto generados..."

# Borrar archivos compilados de C
rm -f 01_hello_printf
rm -f 02_hello_syscall
rm -f 03_fork_example
rm -f 04_write_file
rm -f 05_read_file

# Borrar archivos objeto y ejecutables de Ensamblador
rm -f 01_hello_printf.o
rm -f 02_hello_syscall.o
rm -f 03_fork_example.o
rm -f 04_write_file.o
rm -f 05_read_file.o
rm -f 01_hello_printf_asm
rm -f 02_hello_syscall_asm
rm -f 03_fork_example_asm
rm -f 04_write_file_asm
rm -f 05_read_file_asm

# Borrar los archivos de texto de prueba creados por los ejemplos 04 y 05
rm -f archivo_ejemplo.txt
rm -f archivo_ejemplo_asm.txt
# (Opcional) Borrar nuestro binario experimental 'edi' si existiera
rm -f edi

echo "¡Directorio limpio!"
