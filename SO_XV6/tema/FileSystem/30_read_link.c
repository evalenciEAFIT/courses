#include <stdio.h>
#include <unistd.h>  // Para readlink
#include <stdlib.h>

/**
 * PROGRAMA 30: leer_enlace_simbolico.c (readlink)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * El contenido de un archivo regular son sus bytes.
 * El contenido de un enlace simbólico es la RUTA a la que apunta.
 * 
 * readlink() devuelve esa ruta pero, ATENCIÓN, ¡no añade un '\0' al final!
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <enlace_simbolico_a_leer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *enlace = argv[1];
    char buffer[256]; // Aquí guardaremos la ruta resuelta

    // 1. Leer el contenido del enlace
    // readlink(ruta_enlace, buffer, tamaño_buffer)
    // Devuelve cuántos bytes de la ruta leyó (no es un string C estándar).
    ssize_t bytes_leidos = readlink(enlace, buffer, sizeof(buffer) - 1);

    if (bytes_leidos == -1) {
        perror("Fallo al leer el contenido del enlace");
        return EXIT_FAILURE;
    }

    // 2. IMPORTANTE: El C estándar requiere que los strings terminen en \0
    buffer[bytes_leidos] = '\0';

    printf("--- Análisis de Enlace Simbólico ---\n");
    printf("> Nombre del enlace: %s\n", enlace);
    printf("> Destino (ruta):     %s\n", buffer);

    printf("\n¡Felicidades! Has completado el bloque de Metadatos y Enlaces (21-30).\n");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (DETALLE TÉCNICO): 
 * Si pasas el nombre de un archivo REGULAR a readlink(), fallará.
 * Esta función es exclusiva para "resolver" punteros del sistema de archivos.
 */
