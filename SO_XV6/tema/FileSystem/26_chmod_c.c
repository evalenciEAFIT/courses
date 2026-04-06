#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
 * PROGRAMA 26: cambiar_permisos.c (chmod programático)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * Entender cómo las aplicaciones (como un instalador) aseguran que 
 * los archivos tengan los permisos correctos en el disco.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    // Para este programa, necesitamos el nombre del archivo y el modo octal
    if (argc != 2) {
        printf("Uso: %s <archivo_a_proteger>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nombre = argv[1];

    // 1. Queremos darle permisos de "Lectura solo para el dueño" (0400 octal)
    // Usamos chmod(ruta, modo_octal). Importante poner el 0 delante para indicar octal en C.
    mode_t modo_seguro = 0400; // r--------

    printf("Intentando cambiar permisos de %s a 0400 (Solo lectura dueño)...\n", nombre);

    if (chmod(nombre, modo_seguro) == -1) {
        perror("Error al aplicar chmod");
        return EXIT_FAILURE;
    }

    printf("¡Permisos cambiados exitosamente!\n");
    printf("Prueba a escribir en él con: echo 'hola' >> %s\n", nombre);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * El comando shell 'chmod' es en realidad un envoltorio (wrapper)
 * de esta llamada al sistema de C. Por eso puedes manipular 
 * archivos desde tus programas con la misma potencia de la terminal.
 */
