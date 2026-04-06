#include <stdio.h>
#include <unistd.h>  // Para link
#include <stdlib.h>

/**
 * PROGRAMA 28: enlace_fisico.c (Hard Links)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Un Hard Link es "un nombre extra para el mismo Inodo".
 * El archivo NO se duplica en el disco. Solo ahora el Inodo tiene 2 nombres.
 * 
 * Si borras el primer nombre, ¡el archivo sigue existiendo en el segundo!
 * El archivo solo se borra físicamente cuando el contador de enlaces llega a 0.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo_existente>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *existente = argv[1];
    const char *enlace = "nuevo_nombre_fisico.txt";

    // 1. Crear el Hard Link
    // link(origen, destino_enlace)
    if (link(existente, enlace) == -1) {
        perror("Fallo al crear Hard Link");
        return EXIT_FAILURE;
    }

    printf("¡Enlace Físico creado!\n");
    printf("Nombres actuales para el mismo Inodo: %s y %s\n", existente, enlace);
    printf("Prueba a ver los Inodos con el comando: ls -i %s %s\n", existente, enlace);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTRUCTURA DISCO): 
 * Un directorio es técnicamente una tabla [Nombre | Inodo].
 * Al crear un Hard Link, simplemente se añade una fila más con el 
 * nuevo nombre pero el MISMO número de Inodo.
 */
