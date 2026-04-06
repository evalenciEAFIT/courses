#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 03: leer_caracter.c
 * Concepto: Lectura de archivos caracter a caracter usando `fgetc()`.
 *           Detección del Fin de Archivo (EOF).
 */

int main() {
    FILE *archivo = fopen("caracteres.txt", "r");
    
    if (archivo == NULL) {
        printf("Error: Ejecuta primero '02_escribir_caracter.c' para crear el archivo.\n");
        perror("Ocurrió el siguiente error");
        return EXIT_FAILURE;
    }
    
    int caracter; // IMPORTANTE usar 'int' para capturar EOF (que suele ser -1)
    
    printf("Contenido de 'caracteres.txt':\n");

    // Leemos hasta que llegamos al final (EOF)
    while ((caracter = fgetc(archivo)) != EOF) {
        printf("%c ", (char)caracter); // Mostramos el caracter leído
    }
    
    printf("\n\nLlegamos al final del archivo.\n");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
