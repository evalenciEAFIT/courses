#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define BYTES_TO_WRITE 1000000 // 1 Millón de bytes
#define BUFFER_SIZE 4096      // Tamaño del buffer (4 KB)

int main() {
    int fd = open("output_syscall_buf.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Escribimos usando nuestro propio buffer para minimizar llamadas al sistema
    char buffer[BUFFER_SIZE];
    memset(buffer, 'A', BUFFER_SIZE);

    int remaining = BYTES_TO_WRITE;
    while (remaining > 0) {
        int to_write = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;
        write(fd, buffer, to_write);
        remaining -= to_write;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    close(fd);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("3. System Call (+Buffer 4KB)     -> Tiempo: %f segundos\n", time_taken);
    return 0;
}
