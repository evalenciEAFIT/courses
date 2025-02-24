# Explorando la Memoria en C++: Variables, Pila y Montón

Este programa en C++ ilustra cómo se almacenan las variables en diferentes áreas de memoria (estática, pila y montón) y cómo verificar su ubicación.

## Código

```c++
#include <iostream>
#include <unistd.h>    // Para sbrk (aunque no se usa directamente en este ejemplo)
#include <cstdint>     // Para uintptr_t
#include <cstring>    // Para strstr
#include <iomanip>    // Para setw()
#include <cstdlib>   // Para atoi

// Códigos de escape ANSI para colores con fondo y texto
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RED_BG  "\033[41m"
#define GREEN_BG  "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG   "\033[44m"
#define BLACK_BG  "\033[40m"
#define WHITE   "\033[97m"

// Función para verificar si una dirección está en el heap
bool isAddressInHeap(void* ptr) {
    //... (Código de la función isAddressInHeap)...
}

// Función para verificar si una dirección está en la pila (stack)
bool isAddressInStack(void* ptr) {
    //... (Código de la función isAddressInStack)...
}

int globalVariable = 33; // Variable global, almacenada en la sección de datos estáticos

int main(int argc, char** argv) { 
    if (argc > 1) {
        if (strcmp(argv, "-h") == 0 || strcmp(argv, "--help") == 0) {
            std::cout << "Uso: " << argv << " [valor_variable_global]" << std::endl;
            std::cout << "   ´El valor por defecto de la variable global es 33´" << std::endl;
            std::cout << "  -h, --help       Mostrar esta ayuda." << std::endl;
            std::cout << "  valor_variable_global  Establece el valor de la variable global." << std::endl;
            return 0;
        } else {
            globalVariable = atoi(argv); 
        }
    }

    int localVariable;
    int* heapVariable = new int;
    static int staticLocalVariable;

    // Mostrar las direcciones y los valores de las variables con colores
    //... (Código para imprimir la información de las variables)...

    // Realizar verificaciones y mostrar los resultados con colores
    //... (Código para imprimir las verificaciones)...

    delete heapVariable;

    return 0;
}
