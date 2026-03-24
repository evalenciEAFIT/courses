#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTES_TO_WRITE 1000000 // 1 Millón de bytes

int main() {
    FILE *file = fopen("output_lib.txt", "w");
    if (file == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Escribimos 1 byte a la vez usando fputc
    // fputc es parte de la librería estándar de C y tiene un búfer interno
    for (int i = 0; i < BYTES_TO_WRITE; i++) {
        fputc('A', file);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    fclose(file);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("1. Librería C Estándar (fputc)   -> Tiempo: %f segundos\n", time_taken);
    return 0;
}
