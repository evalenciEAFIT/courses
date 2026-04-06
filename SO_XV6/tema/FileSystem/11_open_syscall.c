#include <stdio.h>
#include <fcntl.h>   // Para open y sus flags
#include <unistd.h>  // Para close
#include <stdlib.h>

/**
 * Programa 11: open_syscall.c
 * Concepto: Abrir un archivo usando la llamada al sistema 'open'.
 *           Retorna un descriptor de archivo (fd), no un stream FILE*.
 */

int main() {
    int fd;
    
    // Abrimos para escritura (O_WRONLY), creando el archivo (O_CREAT)
    // y truncándolo si ya existe (O_TRUNC).
    // El tercer parámetro son los permisos (0644 -> rw-r--r--)
    fd = open("syscall_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Error al abrir con syscall");
        return EXIT_FAILURE;
    }
    
    printf("Archivo abierto exitosamente. File Descriptor (fd): %d\n", fd);
    
    if (close(fd) == -1) {
        perror("Error al cerrar");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
