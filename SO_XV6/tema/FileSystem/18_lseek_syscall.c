#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * PROGRAMA 18: lseek_syscall.c (Llamada al Sistema Directa)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * A bajo nivel, ftell() y fseek() no existen. El kernel de Linux usa lseek().
 * 
 * ¿Por qué "l"seek?
 * La 'l' viene de "long-offset seek", indicando que puede manejar archivos
 * de más de 2GB (usando offsets de 64 bits modernos).
 * --------------------------------------------------------------------------
 */

int main() {
    int fd = open("datos_syscall.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return EXIT_FAILURE;

    // 1. Escribimos 10 bytes de datos nulos (0)
    write(fd, "0123456789", 10);
    
    // 2. Saltar a la posición 5 usando lseek()
    // off_t lseek(int fd, off_t offset, int whence);
    // Retorna la nueva posición absoluta desde el inicio (o -1 si hay error).
    off_t posicionFinal = lseek(fd, 5, SEEK_SET);

    if (posicionFinal == -1) {
        perror("Fallo en el salto de lseek");
        return EXIT_FAILURE;
    }

    // 3. Sobrescribimos el byte en esa posición exacta
    write(fd, "X", 1); 

    printf("Se ha saltado a la posición %ld y se ha escrito una 'X'.\n", posicionFinal);
    printf("Revisa el archivo con el comando: cat datos_syscall.bin\n");
    printf("Resultado esperado: 01234X6789\n");

    close(fd);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (EL KERNEL): 
 * El kernel mantiene una tabla de archivos abiertos. Cada entrada tiene un 
 * "offset" interno. lseek() simplemente cambia ese número en la tabla. 
 * ¡No se lee el disco al llamar a lseek()! Por eso es extremadamente rápido.
 */
