#include <stdio.h>
#include <unistd.h>  // Para rmdir
#include <stdlib.h>

/**
 * PROGRAMA 33: eliminar_directorio.c (rmdir)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * Entender que, por seguridad, Linux solo permite borrar una carpeta
 * usando rmdir() si NO tiene archivos dentro.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre_directorio_a_borrar>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nombre = argv[1];

    printf("Intentando eliminar directorio: %s\n", nombre);

    // 1. Eliminar carpeta
    // Si la carpeta tiene archivos, rmdir devolverá -1 y pondrá errno en "Directory not empty".
    if (rmdir(nombre) == -1) {
        perror("Error al borrar directorio");
        return EXIT_FAILURE;
    }

    printf("¡Carpeta '%s' eliminada exitosamente!\n", nombre);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Para un borrado RECURSIVO (como rm -rf), un programa tendría que:
 * 1. Abrir la carpeta.
 * 2. Borrar todos los archivos que hay dentro.
 * 3. Luego, final y únicamente ahí, llamar a rmdir().
 */
