#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * PROGRAMA 14: duplicar_fd.c
 * --------------------------------------------------------------------------
 * CONCEPTO PROFUNDO: 
 * En Linux, cada archivo abierto tiene un "Número de Identificación" (File Descriptor).
 * Al usar `dup()`, el Sistema Operativo crea una copia exacta de ese identificador.
 * Ambos descriptores apuntan a la misma "Entrada de Archivo Abierto" en el Kernel.
 * Esto significa que si mueves el cursor en uno, también se mueve en el otro.
 * --------------------------------------------------------------------------
 */

int main() {
    // 1. Abrimos un archivo para escritura.
    // O_RDWR: Lectura y Escritura.
    // O_CREAT: Crear si no existe.
    // 0644: Permisos - El dueño lee/escribe, el resto solo lee.
    int fd_original = open("duplicado.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    
    if (fd_original == -1) {
        perror("Error al abrir el original");
        return EXIT_FAILURE;
    }

    // 2. Duplicamos el descriptor.
    // dup() buscará el número más pequeño disponible para el nuevo descriptor.
    int fd_copia = dup(fd_original);
    
    if (fd_copia == -1) {
        perror("Error al duplicar descriptor");
        close(fd_original);
        return EXIT_FAILURE;
    }

    printf("Original FD: %d, Copia FD: %d\n", fd_original, fd_copia);

    // 3. Escribimos usando el descriptor ORIGINAL.
    const char *msg1 = "Escrito por el original.\n";
    write(fd_original, msg1, strlen(msg1));

    // 4. Escribimos usando el descriptor COPIA.
    // Como comparten el mismo "offset" (puntero de posición), el texto se escribirá
    // justo después del mensaje anterior, NO lo sobrescribirá.
    const char *msg2 = "Escrito por la copia.\n";
    write(fd_copia, msg2, strlen(msg2));

    printf("Revisa 'duplicado.txt' para ver que ambos escribieron en secuencia.\n");

    // 5. ¡Importante! Cerramos ambos descriptores para ser buenos ciudadanos del SO.
    close(fd_original);
    close(fd_copia);

    return EXIT_SUCCESS;
}
