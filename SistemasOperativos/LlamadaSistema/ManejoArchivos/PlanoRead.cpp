#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Abrir el archivo en modo solo lectura.
    int fd = open("archivo.txt", O_RDONLY);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error abriendo el archivo");
        return 1;
    }
    
    // Declarar un buffer para almacenar los datos leídos del archivo.
    char buffer[128];
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer));
    
    // Escribir los datos leídos directamente en la salida estándar.
    write(STDOUT_FILENO, buffer, bytes_leidos);
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}