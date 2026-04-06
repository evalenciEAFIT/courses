#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 02: escribir_caracter.c
 * Concepto: Uso de `fputc()` para escribir caracter a caracter en un archivo.
 */

int main() {
    FILE *archivo = fopen("caracteres.txt", "w");
    
    if (archivo == NULL) {
        perror("Error al abrir para escritura");
        return EXIT_FAILURE;
    }
    
    // Escribir "A" a "Z" uno por uno
    for (char c = 'A'; c <= 'Z'; c++) {
        if (fputc(c, archivo) == EOF) {
            perror("Error al escribir el caracter");
            fclose(archivo);
            return EXIT_FAILURE;
        }
    }
    
    // También podemos escribir saltos de línea (un caracter especial)
    fputc('\n', archivo);
    
    printf("Alfabeto escrito exitosamente en 'caracteres.txt'.\n");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
