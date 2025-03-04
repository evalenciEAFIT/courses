#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Función para leer la memoria del proceso y mostrar información relevante
void escanearMemoria(pid_t pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/maps";
    std::ifstream archivo(path);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de memoria del proceso." << std::endl;
        return;
    }
    
    std::cout << "Escaneo de memoria del proceso " << pid << "\n";
    std::cout << "-----------------------------------------------------\n";
    
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }
    
    archivo.close();
}

int main() {
    pid_t pid = getpid(); // Obtener el ID del proceso actual
    std::cout << "Proceso actual: " << pid << "\n";
    
    escanearMemoria(pid);
    
    return 0;
}
