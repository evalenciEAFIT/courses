#!/bin/bash

# Compilar los programas
echo "Compilando programas..."
gcc -O2 lib_io.c -o lib_io
gcc -O2 syscall_io.c -o syscall_io
gcc -O2 syscall_buffered.c -o syscall_buffered

echo "--------------------------------------------------------"
echo "Ejecutando la prueba de rendimiento E/S (1 Millón de Bytes)"
echo "--------------------------------------------------------"

# Ejecutar los programas y mostrar tiempos
./lib_io
./syscall_io
./syscall_buffered

echo "--------------------------------------------------------"
echo "Archivos de salida generados:"
ls -lh output_lib.txt output_syscall.txt output_syscall_buf.txt
echo ""
echo "Primeros 5 bytes de cada archivo:"
echo -n "  output_lib.txt      -> "; head -c 5 output_lib.txt; echo
echo -n "  output_syscall.txt  -> "; head -c 5 output_syscall.txt; echo
echo -n "  output_syscall_buf.txt -> "; head -c 5 output_syscall_buf.txt; echo
echo "--------------------------------------------------------"
echo "Prueba terminada. Archivos conservados."

