#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Programa 09: contar_palabras.c
 * Concepto: Analizar el contenido de un archivo (como el comando `wc`).
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    int c;
    long lineas = 0, palabras = 0, caracteres = 0;
    int en_palabra = 0;

    while ((c = fgetc(archivo)) != EOF) {
        caracteres++;
        if (c == '\n') lineas++;
        
        if (isspace(c)) {
            en_palabra = 0;
        } else if (!en_palabra) {
            en_palabra = 1;
            palabras++;
        }
    }

    printf("Resultados para '%s':\n", argv[1]);
    printf("Líneas: %ld\n", lineas);
    printf("Palabras: %ld\n", palabras);
    printf("Caracteres: %ld\n", caracteres);

    fclose(archivo);

    return EXIT_SUCCESS;
}
