#include <stdio.h>
#include <unistd.h>  // Para symlink
#include <stdlib.h>

/**
 * PROGRAMA 29: enlace_simbolico.c (Symbolic Links)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Un Symbolic Link (o Soft Link) NO apunta al Inodo. 
 * Apunta a la RUTA de otro archivo (un puntero a texto).
 * 
 * Si borras el archivo original, el Symbolic Link queda roto (Dangling Link).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    const char *origen = argv[1];
    const char *enlace_simbolico = "mi_acceso_directo.lnk";

    // 1. Crear el Enlace Simbólico
    // symlink(ruta_original, ruta_enlace)
    if (symlink(origen, enlace_simbolico) == -1) {
        perror("Fallo al crear Symbolic Link");
        return EXIT_FAILURE;
    }

    printf("¡Enlace Simbólico creado!\n");
    printf("Ruta original: %s\n", origen);
    printf("Nombre del enlace: %s\n", enlace_simbolico);
    
    printf("Prueba a ver la información con: ls -l %s\n", enlace_simbolico);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTILO DE APRENDIZAJE): 
 * Los enlaces simbólicos pueden apuntar a archivos que están en 
 * OTROS discos o sistemas de archivos (porque solo guardan la ruta).
 * ¡Los Hard Links solo funcionan dentro del MISMO disco!
 */
