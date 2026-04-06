#include <stdio.h>
#include <unistd.h>  // Para chown
#include <sys/types.h>
#include <stdlib.h>

/**
 * PROGRAMA 27: cambiar_dueño.c (chown programático)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO (IMPORTANTE):
 * Cambiar el dueño de un archivo requiere privilegios de SUPERUSUARIO (ROOT).
 * Si no eres Root, el comando fallará.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo_a_cambiar_dueño>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nombre = argv[1];

    // uid_t y gid_t son tipos especiales de Linux para los IDs del sistema.
    // 0 suele ser el ID del usuario Root.
    uid_t nuevo_dueño = 0; 
    gid_t nuevo_grupo = 0; 

    printf("Intentando cambiar dueño de %s a ROOT (ID 0)...\n", nombre);

    if (chown(nombre, nuevo_dueño, nuevo_grupo) == -1) {
        printf("--- Error en chown ---\n");
        perror("Fallo en cambiar el dueño (¿Tienes permisos de sudo?)");
        return EXIT_FAILURE;
    }

    printf("¡Dueño cambiado exitosamente a ROOT!\n");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (SEGURIDAD): 
 * Si cualquier usuario pudiera cambiar el dueño de un archivo, 
 * podría robarle la autoría a otros. Por eso el SO impone esta 
 * restricción desde el kernel.
 */
