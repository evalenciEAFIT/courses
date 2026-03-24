#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BYTES_TO_WRITE 1000000 // 1 Millón de bytes

int main() {
    // Abrimos archivo con O_WRONLY (solo escritura), O_CREAT (crear si no existe), 
    // O_TRUNC (truncar modo) y permisos 0644
    int fd = open("output_syscall.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Escribimos 1 byte a la vez usando 'write' system call
    // ESTO PROVOCA un cambio al 'Kernel Mode' (Context Switch) 1 Millón de veces.
    // Va a ser MUY lento!
    char c = 'A';
    for (int i = 0; i < BYTES_TO_WRITE; i++) {
        write(fd, &c, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    close(fd);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("2. System Call puros (write 1b)  -> Tiempo: %f segundos\n", time_taken);
    return 0;
}
