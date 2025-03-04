#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Definición de colores ANSI para mejorar la visualización
#define RESET     "\033[0m"
#define RED       "\033[31m" // Código ejecutable (TEXT)
#define GREEN     "\033[32m" // Heap
#define YELLOW    "\033[33m" // Data (BSS, variables globales)
#define BLUE      "\033[34m" // Stack
#define MAGENTA   "\033[35m" // Librerías compartidas
#define CYAN      "\033[36m" // Espacio en memoria compartida
#define WHITE     "\033[37m" // Regiones especiales (VDSO, VVAR, etc.)

// Función para leer la memoria del proceso y clasificar sus regiones con color
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
        std::string color = RESET; // Color por defecto

        // Clasificación de las regiones de memoria y asignación de color
        if (linea.find("[heap]") != std::string::npos) {
            color = GREEN;
            std::cout << color << linea << "  --> Región de memoria HEAP" << RESET << std::endl;
        } else if (linea.find("[stack]") != std::string::npos) {
            color = BLUE;
            std::cout << color << linea << "  --> Región de memoria STACK" << RESET << std::endl;
        } else if (linea.find("[vdso]") != std::string::npos) {
            color = WHITE;
            std::cout << color << linea << "  --> Página VDSO (Virtual Dynamic Shared Object)" << RESET << std::endl;
        } else if (linea.find("[vvar]") != std::string::npos) {
            color = WHITE;
            std::cout << color << linea << "  --> Página VVAR (Variables del Kernel)" << RESET << std::endl;
        } else if (linea.find("[vsyscall]") != std::string::npos) {
            color = WHITE;
            std::cout << color << linea << "  --> Página VSYSCALL (Llamadas al sistema rápidas)" << RESET << std::endl;
        } else if (linea.find(".so") != std::string::npos) {
            color = MAGENTA;
            std::cout << color << linea << "  --> Librería compartida (SO)" << RESET << std::endl;
        } else if (linea.find("/usr/lib") != std::string::npos) {
            color = MAGENTA;
            std::cout << color << linea << "  --> Biblioteca del sistema" << RESET << std::endl;
        } else if (linea.find("r-xp") != std::string::npos) {
            color = RED;
            std::cout << color << linea << "  --> Segmento de código ejecutable (TEXT)" << RESET << std::endl;
        } else if (linea.find("rw-p") != std::string::npos) {
            color = YELLOW;
            std::cout << color << linea << "  --> Segmento de datos (DATA o BSS)" << RESET << std::endl;
        } else {
            std::cout << linea << std::endl; // Línea sin clasificación específica
        }
    }
    
    archivo.close();
}

int main() {
    pid_t pid = getpid(); // Obtener el ID del proceso actual
    std::cout << "Proceso actual: " << pid << "\n";
    
    escanearMemoria(pid);
    
    return 0;
}
