#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * Programa 12: read_write_syscall.c
 * Concepto: Escribir y leer bloques de bytes directamente desde el núcleo
 *           del sistema operativo con 'read()' y 'write()'.
 */

int main() {
    int fd;
    const char *mensaje = "Hola desde las entrañas de Linux Syscalls!\n";
    char buffer[100];
    ssize_t bytes;

    // 1. Escritura
    fd = open("syscall_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return EXIT_FAILURE;

    // write(fd, buffer, cantidad_bytes)
    // Retorna la cantidad de bytes que realmente pudo escribir
    bytes = write(fd, mensaje, strlen(mensaje));
    printf("Escritos %ld bytes.\n", bytes);
    close(fd);

    // 2. Lectura
    fd = open("syscall_test.txt", O_RDONLY);
    if (fd == -1) return EXIT_FAILURE;

    // read(fd, buffer, bytes_a_leer)
    // Retorna 0 al llegar al final del archivo (EOF)
    bytes = read(fd, buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0'; // Agregamos el terminador nulo para imprimir

    printf("Leído desde archivo con descriptor %d: \n> %s", fd, buffer);
    
    close(fd);

    return EXIT_SUCCESS;
}
