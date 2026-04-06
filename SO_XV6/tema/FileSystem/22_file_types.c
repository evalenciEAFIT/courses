#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
 * PROGRAMA 22: tipos_archivos.c (IDENTIFICACIÓN)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * En Linux, "Todo es un archivo". Pero, ¿cómo diferenciarlos?
 * 
 * En el campo 'st_mode' del struct stat, se guardan varios bits
 * que nos dicen si el objeto es una carpeta, archivo o algo más.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    struct stat st;
    if (stat(argv[1], &st) == -1) return EXIT_FAILURE;

    printf("Analizando tipo de objeto: %s\n", argv[1]);

    // Usaremos MACROS de conveniencia definidas en <sys/stat.h>
    // Se pasan el valor st_mode y nos dan un resultado booleano.

    if (S_ISREG(st.st_mode)) {
        printf("> Es un archivo REGULAR (como un .txt, .c, o binario).\n");
    } else if (S_ISDIR(st.st_mode)) {
        printf("> Es un DIRECTORIO (una carpeta).\n");
    } else if (S_ISLNK(st.st_mode)) {
        printf("> Es un ENLACE SIMBÓLICO (shortcut).\n");
    } else if (S_ISCHR(st.st_mode)) {
        printf("> Es un dispositivo de CARACTER (como tu teclado/terminal).\n");
    } else if (S_ISBLK(st.st_mode)) {
        printf("> Es un dispositivo de BLOQUE (como el disco duro).\n");
    } else if (S_ISFIFO(st.st_mode)) {
        printf("> Es una FIFO / PIPE (comunicación entre procesos).\n");
    } else {
        printf("> Tipo de archivo desconocido.\n");
    }

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (DETRÁS DE ESCENA): 
 * El kernel de Linux no mira la "extensión" (.txt) para saber el tipo.
 * Mira los bits 12 a 15 del campo 'st_mode' del Inodo.
 * Ahí es donde reside la "verdad" de qué cosa es un archivo.
 */
