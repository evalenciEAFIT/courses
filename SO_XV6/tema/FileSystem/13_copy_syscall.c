#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Programa 13: copy_syscall.c
 * Concepto: Implementar la copia de archivos por bloques usando solo
 *           read() y write(). Es mucho más rápido que fgetc()/fputc().
 */

#define BUFFER_SIZE 4096 // Un bloque típico de disco (4KB)

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <origen> <destino>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int src_fd = open(argv[1], O_RDONLY);
    int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (src_fd == -1 || dst_fd == -1) {
        perror("Error en apertura");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_leidos;

    // Bucle de copia por bloques
    while ((bytes_leidos = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dst_fd, buffer, bytes_leidos) != bytes_leidos) {
            perror("Error en escritura parcial");
            break;
        }
    }

    if (bytes_leidos == -1) perror("Error al leer");
    
    printf("Copia por bloques (4KB) completada: %s -> %s\n", argv[1], argv[2]);

    close(src_fd);
    close(dst_fd);

    return EXIT_SUCCESS;
}
