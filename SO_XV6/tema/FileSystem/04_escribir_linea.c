#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 04: escribir_linea.c
 * Concepto: Escribir cadenas completas de texto usando `fputs()`.
 */

int main() {
    FILE *archivo = fopen("notas.txt", "w");
    
    if (archivo == NULL) {
        perror("Error al abrir notas.txt");
        return EXIT_FAILURE;
    }
    
    // fputs escribe la cadena indicada (sin añadir un salto de línea \n)
    fputs("Línea 1: Esta es una prueba de escritura de líneas.\n", archivo);
    fputs("Línea 2: El manejo de archivos en C es potente y eficiente.\n", archivo);
    fputs("Línea 3: Linux permite gestionar todo como archivos.\n", archivo);
    
    printf("Varias líneas se han guardado en 'notas.txt'.\n");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
