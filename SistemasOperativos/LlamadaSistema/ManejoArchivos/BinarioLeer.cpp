#include <fcntl.h>    // Para open
#include <unistd.h>   // Para read, write, close
#include <sys/stat.h> // Para permisos de archivo
#include <string.h>   // Para strlen
#include <stdio.h>    // Para printf

/**
 * @brief Programa que lee un archivo binario y muestra cada byte en ASCII, binario y hexadecimal.
 *        Usa exclusivamente llamadas al sistema en Linux.
 */
int main() {
    const char* nombre_archivo = "salida.bin"; // Nombre del archivo binario

    // Abrir el archivo en modo lectura binaria
    int fd = open(nombre_archivo, O_RDONLY);
    if (fd == -1) {
        write(STDERR_FILENO, "Error al abrir el archivo binario para lectura.\n", 47);
        return 1;
    }

    unsigned char byte; // Almacena cada byte leído
    ssize_t bytes_leidos;

    // Encabezado
    const char* encabezado = "Caracter :: Binario      :: Hexadecimal\n-------------------------------------\n";
    write(STDOUT_FILENO, encabezado, strlen(encabezado));

    // Leer byte por byte
    while ((bytes_leidos = read(fd, &byte, 1)) > 0) {
        char buffer_salida[50]; // Buffer para almacenar la línea de salida
        char caracter = (byte >= 32 && byte <= 126) ? byte : '.'; // Caracter imprimible o '.'
        
        // Convertir a binario manualmente
        char binario[9]; // 8 bits + terminador
        for (int i = 7; i >= 0; --i)
            binario[7 - i] = ((byte >> i) & 1) ? '1' : '0';
        binario[8] = '\0'; // Terminar la cadena

        // Formatear la salida en buffer (usamos sprintf porque write no permite formato directo)
        int len = sprintf(buffer_salida, "  %c      :: %s :: 0x%02X\n", caracter, binario, byte);

        // Escribir la línea de salida en la terminal
        write(STDOUT_FILENO, buffer_salida, len);
    }

    if (bytes_leidos == -1) {
        write(STDERR_FILENO, "Error al leer el archivo binario.\n", 34);
        close(fd);
        return 1;
    }

    // Cerrar el archivo
    close(fd);

    return 0;
}
