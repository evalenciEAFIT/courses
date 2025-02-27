#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Función para obtener el tamaño de un archivo
off_t obtener_tamano_archivo(const char *ruta) {
    struct stat sb;
    if (stat(ruta, &sb) == -1) {
        perror("Error obteniendo tamaño del archivo");
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ruta_archivo = argv[1];
    off_t tamano_archivo = obtener_tamano_archivo(ruta_archivo);

    std::cout << "[INFO] Ejecutando comando ls -lh sobre el archivo." << std::endl;
    std::string comando = "ls -lh " + std::string(ruta_archivo);
    system(comando.c_str());
    
    int fd = open(ruta_archivo, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return EXIT_FAILURE;
    }

    // Mapear archivo en memoria
    char *data = (char *)mmap(NULL, tamano_archivo, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Error al mapear archivo en memoria");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd); // El descriptor ya no es necesario después del mapeo
    
    std::cout << "[INFO] Archivo mapeado en memoria correctamente." << std::endl;

    // Mostrar los primeros 100 caracteres del archivo
    std::cout << "[INFO] Primeros 100 caracteres del archivo:" << std::endl;
    for (size_t i = 0; i < 100 && i < tamano_archivo; ++i) {
        std::cout << data[i];
    }
    std::cout << std::endl;

    // Liberar la memoria mapeada
    if (munmap(data, tamano_archivo) == -1) {
        perror("Error al liberar memoria mapeada");
    }

    std::cout << "[INFO] Memoria liberada y procesamiento finalizado." << std::endl;

    return EXIT_SUCCESS;
}
