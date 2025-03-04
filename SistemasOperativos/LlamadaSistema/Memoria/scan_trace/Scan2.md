# Escaneo de Memoria de un Proceso en Linux

## Introducción
El manejo de memoria es fundamental en los sistemas operativos modernos. En Linux, cada proceso tiene una estructura de memoria organizada en diferentes segmentos, como **heap, stack, text y data**. Para inspeccionar la memoria de un proceso en ejecución, el sistema operativo proporciona el archivo `/proc/<pid>/maps`, que contiene información detallada sobre la distribución de memoria del proceso.

Este documento describe la implementación de un programa en **C++** que analiza el archivo `/proc/<pid>/maps`, clasifica las regiones de memoria y las visualiza con colores para facilitar su interpretación.

## Justificación
El monitoreo de memoria es crucial para:
- **Depurar programas** y detectar fugas de memoria.
- **Comprender la asignación de memoria** en aplicaciones.
- **Optimizar el uso de memoria** en software de alto rendimiento.
- **Aprender sobre la organización de memoria** en sistemas operativos Linux.

Este programa permite analizar de forma visual y clasificada las regiones de memoria utilizadas por un proceso, identificando secciones como:
- **Heap**: Memoria asignada dinámicamente mediante `malloc` o `new`.
- **Stack**: Memoria de la pila donde se almacenan variables locales y direcciones de retorno.
- **Text**: Sección del código ejecutable del programa.
- **Data/BSS**: Segmentos donde se almacenan variables globales y estáticas.
- **Librerías compartidas (.so)**: Código externo cargado en memoria.
- **Regiones especiales**: Segmentos asignados por el sistema como `VDSO`, `VVAR`, `VSYSCALL`.

## Implementación en C++
Se utiliza C++ para leer y analizar `/proc/<pid>/maps`. Se han agregado **códigos de color ANSI** para resaltar cada tipo de memoria:
- **Heap**: 🟩 Verde
- **Stack**: 🔵 Azul
- **Texto (código ejecutable)**: 🔴 Rojo
- **Data/BSS**: 🟨 Amarillo
- **Librerías compartidas**: 🔷 Magenta
- **Regiones especiales**: ⚪ Blanco

### Código fuente en C++
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RESET     "\033[0m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define BLUE      "\033[34m"
#define MAGENTA   "\033[35m"
#define WHITE     "\033[37m"

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
        std::string color = RESET;

        if (linea.find("[heap]") != std::string::npos) {
            color = GREEN;
            std::cout << color << linea << "  --> HEAP" << RESET << std::endl;
        } else if (linea.find("[stack]") != std::string::npos) {
            color = BLUE;
            std::cout << color << linea << "  --> STACK" << RESET << std::endl;
        } else if (linea.find("[vdso]") != std::string::npos || linea.find("[vvar]") != std::string::npos || linea.find("[vsyscall]") != std::string::npos) {
            color = WHITE;
            std::cout << color << linea << "  --> REGIÓN ESPECIAL" << RESET << std::endl;
        } else if (linea.find(".so") != std::string::npos || linea.find("/usr/lib") != std::string::npos) {
            color = MAGENTA;
            std::cout << color << linea << "  --> LIBRERÍA COMPARTIDA" << RESET << std::endl;
        } else if (linea.find("r-xp") != std::string::npos) {
            color = RED;
            std::cout << color << linea << "  --> SEGMENTO DE CÓDIGO (TEXT)" << RESET << std::endl;
        } else if (linea.find("rw-p") != std::string::npos) {
            color = YELLOW;
            std::cout << color << linea << "  --> SEGMENTO DE DATOS (DATA/BSS)" << RESET << std::endl;
        } else {
            std::cout << linea << std::endl;
        }
    }
    archivo.close();
}

int main() {
    pid_t pid = getpid();
    std::cout << "Proceso actual: " << pid << "\n";
    escanearMemoria(pid);
    return 0;
}
```

## Conclusiones
Este programa proporciona una forma rápida y efectiva de analizar la memoria de un proceso en Linux. Gracias a la clasificación por colores, se facilita la interpretación de las regiones de memoria, lo cual es útil para depuración, optimización y aprendizaje sobre el manejo de memoria en sistemas operativos.

### Beneficios
✅ Permite visualizar en tiempo real la memoria del proceso.
✅ Facilita la identificación de regiones de memoria importantes.
✅ Ayuda a detectar problemas de asignación y uso de memoria.
✅ Es útil en entornos de desarrollo y depuración.

Este código puede ser extendido para incluir análisis más detallados, como el tamaño de cada segmento de memoria o el uso de `mmap` por parte del proceso. 

💪 **Optimiza tu código y mejora el rendimiento de tus programas conociendo su uso de memoria!** 🚀
