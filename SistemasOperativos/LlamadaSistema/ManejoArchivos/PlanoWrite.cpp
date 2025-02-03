
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

int main() {
    // Abrir el archivo en modo escritura, creándolo si no existe y truncándolo si ya existe.
    int fd = open("archivo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error creando el archivo");
        return 1;
    }
    
    // Solicitar al usuario que ingrese un texto para guardar en el archivo.
    std::string entrada;
    std::cout << "Ingrese el texto a guardar: ";
    std::getline(std::cin, entrada);
    
    // Escribir el texto ingresado en el archivo.
    write(fd, entrada.c_str(), entrada.size());
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}