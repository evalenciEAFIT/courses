#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

/**
 * PROGRAMA 32: syscall_vs_library.c (BENCHMARK)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * Demostrar por qué usar librerías (stdio.h) suele ser mejor que 
 * usar syscalls directas para pequeñas cantidades de datos.
 * 
 * ¿Por qué esta diferencia? 
 * - Las SYS-CALLS requieren un CONTEXT SWITCH (Cambio de Modo Usuario a Kernel).
 * - Las LIBRERÍAS (fwrite/fprintf) usan un BUFFER INTERNO que acumula datos
 *   y solo llama al Kernel cuando el buffer se llena.
 * --------------------------------------------------------------------------
 */

#define TOTAL_BYTES 1000000 // 1 Millón de bytes (1 MB)

void prueba_syscall() {
    int fd = open("output_sys.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char c = 'A';
    for (int i = 0; i < TOTAL_BYTES; i++) {
        write(fd, &c, 1); // 1,000,000 de llamadas directas al kernel
    }
    close(fd);
}

void prueba_libreria() {
    FILE *f = fopen("output_lib.bin", "w");
    char c = 'A';
    for (int i = 0; i < TOTAL_BYTES; i++) {
        fputc(c, f); // 1,000,000 de llamadas a la LIBRERÍA (mucho más rápido)
    }
    fclose(f);
}

int main() {
    clock_t inicio, fin;
    double tiempo_sys, tiempo_lib;

    printf("--- Iniciando Duelo: Syscall vs Library (%d bytes) ---\n", TOTAL_BYTES);

    // 1. Cronometrar SYS-CALLS
    inicio = clock();
    prueba_syscall();
    fin = clock();
    tiempo_sys = ((double) (fin - inicio)) / CLOCKS_PER_SEC;

    // 2. Cronometrar LIBRERÍA
    inicio = clock();
    prueba_libreria();
    fin = clock();
    tiempo_lib = ((double) (fin - inicio)) / CLOCKS_PER_SEC;

    printf("\nResultados:\n");
    printf("> Syscall (write):    %f segundos.\n", tiempo_sys);
    printf("> Librería (fputc): %f segundos.\n", tiempo_lib);
    
    printf("\nREFLEXIÓN ACADÉMICA (ESTILO SO):\n");
    printf("¿Ves la diferencia? La librería es órdenes de magnitud más rápida porque\n");
    printf("la librería STDIO de C acumuló los bytes y solo llamó al Kernel pocas\n");
    printf("veces. En cambio, con la syscall cruzamos al Kernel un millón de veces.\n");

    return EXIT_SUCCESS;
}
