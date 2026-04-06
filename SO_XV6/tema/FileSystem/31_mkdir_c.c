#include <stdio.h>
#include <sys/stat.h> // Para mkdir
#include <sys/types.h>
#include <stdlib.h>

/**
 * PROGRAMA 31: crear_directorio.c (mkdir)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * Un directorio es "un archivo de tipo especial" que el SO gestiona
 * para organizar otros archivos.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre_directorio>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nombre = argv[1];

    // 1. Crear el directorio con permisos octales
    // mkdir(ruta, modo_octal)
    // Usamos 0755 (El dueño hace todo, el resto solo lee/ejecuta).
    if (mkdir(nombre, 0755) == -1) {
        perror("Error al crear directorio");
        return EXIT_FAILURE;
    }

    printf("¡Directorio '%s' creado exitosamente!\n", nombre);
    printf("Compruébalo con el comando: ls -ld %s\n", nombre);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Al crear un directorio, el SO genera automáticamente dos entradas:
 * - '.' (el propio directorio)
 * - '..' (el directorio padre)
 * Por eso un directorio recién creado ya tiene contenido oculto.
 */
