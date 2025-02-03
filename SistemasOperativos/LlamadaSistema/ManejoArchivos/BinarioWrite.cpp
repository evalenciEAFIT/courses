#include <iostream>
#include <fcntl.h>    // Para open
#include <unistd.h>   // Para write, close
#include <cstring>    // Para strlen
#include <sys/stat.h> // Para permisos de archivo

using namespace std;

/**
 * @brief Programa que toma un texto desde el teclado y lo escribe en un archivo binario en Linux.
 *        Usa llamadas al sistema para asegurar la escritura directa en binario.
 */
int main() {
    const char* nombre_archivo = "salida.bin"; // Nombre del archivo binario

    // Abrir el archivo en modo escritura binaria, sin lectura
    int fd = open(nombre_archivo, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        cerr << "Error al abrir el archivo para escritura binaria.\n";
        return 1;
    }

    // Solicitar al usuario que ingrese un texto
    const int BUFFER_SIZE = 256; // Tamaño del buffer para entrada
    char buffer[BUFFER_SIZE];    // Buffer de almacenamiento
    cout << "Ingrese un texto para guardar en el archivo binario: ";
    cin.getline(buffer, BUFFER_SIZE); // Leer el texto desde el teclado

    // Escribir en el archivo binario
    ssize_t bytes_escritos = write(fd, buffer, BUFFER_SIZE);
    if (bytes_escritos != BUFFER_SIZE) {
        cerr << "Error al escribir datos en el archivo binario.\n";
        close(fd);
        return 1;
    }

    cout << "Datos escritos correctamente en " << nombre_archivo << endl;

    // Cerrar el archivo
    close(fd);

    return 0;
}
