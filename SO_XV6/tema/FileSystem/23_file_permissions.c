#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
 * PROGRAMA 23: permisos_archivos.c (PERMISOS rwx)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * Los permisos se guardan en 9 bits (rwx para dueño, grupo y otros).
 * stat() nos devuelve el campo st_mode con todos esos bits habilitados.
 * 
 * En este programa traduciremos los bits a texto amigable (estilo ls -l).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    struct stat st;
    if (stat(argv[1], &st) == -1) return EXIT_FAILURE;

    printf("Analizando permisos de: %s\n", argv[1]);

    printf("> Permisos: ");
    
    // El dueño (User)
    printf((st.st_mode & S_IRUSR) ? "r" : "-");
    printf((st.st_mode & S_IWUSR) ? "w" : "-");
    printf((st.st_mode & S_IXUSR) ? "x" : "-");

    // El grupo (Group)
    printf((st.st_mode & S_IRGRP) ? "r" : "-");
    printf((st.st_mode & S_IWGRP) ? "w" : "-");
    printf((st.st_mode & S_IXGRP) ? "x" : "-");

    // Los demás (Others)
    printf((st.st_mode & S_IROTH) ? "r" : "-");
    printf((st.st_mode & S_IWOTH) ? "w" : "-");
    printf((st.st_mode & S_IXOTH) ? "x" : "-");

    printf("\n");
    
    // Mostramos el valor en octal (muy común en Linux: 644, 755...)
    printf("> Valor en octal: %o\n", st.st_mode & 0777);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTUDIO DE BITS): 
 * El octal se usa porque cada dígito octal (0-7) representa exactamente 3 bits.
 * Por ejemplo, un '7' (binario 111) significa rwx activados. 
 * ¡Es por eso que los administradores de Linux piensan en octal!
 */
