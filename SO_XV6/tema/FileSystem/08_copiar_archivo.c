#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 08: copiar_archivo.c
 * Concepto: Copiar el contenido de un archivo a otro (como el comando `cp`).
 */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <archivo_origen> <archivo_destino>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *origen = fopen(argv[1], "r");
    FILE *destino = fopen(argv[2], "w");

    if (origen == NULL || destino == NULL) {
        perror("Error al abrir archivos");
        return EXIT_FAILURE;
    }

    int c;
    while ((c = fgetc(origen)) != EOF) {
        fputc(c, destino);
    }

    printf("Archivo '%s' copiado exitosamente a '%s'.\n", argv[1], argv[2]);

    fclose(origen);
    fclose(destino);

    return EXIT_SUCCESS;
}
