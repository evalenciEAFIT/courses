#include <iostream>
#include <unistd.h> // Para sbrk
#include <cstdint>  // Para uintptr_t
#include <cstring> // Para strstr
#include <iomanip> // Para setw()

// Códigos de escape ANSI para colores con fondo y texto
#define RESET "\033[0m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define BLACK_BG "\033[40m"

#define WHITE "\033[97m"
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define BLUE "\033[94m"
#define GREEN   "\033[32m"

// Función para verificar si una dirección está en el heap
bool isAddressInHeap(void* ptr) {
    static uintptr_t heapStart = 0;
    static uintptr_t heapEnd = 0;

    if (heapStart == 0) { //Se ejecuta la primera vez para setear los valores.
        // obtener el inicio y fin del heap
        FILE *mapsFile = fopen("/proc/self/maps", "r");
        if (mapsFile == nullptr) {
            std::cerr << "Error al abrir /proc/self/maps" << std::endl;
            return false; // No se puede determinar, se asume que no esta.
        }

        char line[256];
        while (fgets(line, sizeof(line), mapsFile)) {
            uintptr_t start, end;
            char permissions[5];
            char path[128];
            if (sscanf(line, "%lx-%lx %s %*x %*x:%*x %*d %s", &start, &end, permissions, path) == 4) {

                if(strstr(path, "[heap]") != NULL){ //Encuentra la linea correspondiente al heap.
                  heapStart = start;
                  heapEnd = end;
                  break;
                }
            }
        }
        fclose(mapsFile);
    }
    // Verifica si la dirección está dentro del rango del heap.
    uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
    return address >= heapStart && address < heapEnd;
}

// Función para verificar si una dirección está en la pila (stack)
bool isAddressInStack(void* ptr) {
    uintptr_t stackPointer;
    asm("mov %%rsp, %0" : "=r" (stackPointer)); //Obtenemos el stack pointer.

    uintptr_t address = reinterpret_cast<uintptr_t>(ptr);

    //Una forma simplificada es verificar si esta debajo del stack pointer.
    return address <= stackPointer;

    //Una mejor forma seria obtener el rango de la pila de /proc/self/maps, similar a como
    // se obtiene el rango del heap. Pero esta forma simplificada funciona bien para propositos de demo.
}

int globalVariable = 33; // Variable global, almacenada en la sección de datos estáticos.

int main(int argc, char** argv) { 
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) { // Corrección: usar argv
            std::cout << "Uso: " << argv[0] << " [valor_variable_global]" << std::endl;
            std::cout << "   ´El valor por defecto de la variable global es 33´" << std::endl;
            std::cout << "  -h, --help       Mostrar esta ayuda." << std::endl;
            std::cout << "  valor_variable_global  Establece el valor de la variable global." << std::endl;
            return 0;
        } else {
            globalVariable = atoi(argv[1]); 
        }
    }

    int localVariable;
    int* heapVariable = new int;
    //heapVariable[0] = globalVariable;
    static int staticLocalVariable; // = 10;

    std::cout << RESET << "DIRECCION MEMORIA ["  << &globalVariable << RESET << "]\t   "      << BLACK_BG << YELLOW << "STATIC" << RESET << " : " << std::setw(10) << globalVariable       << " variable global "    << std::endl;
    std::cout << RESET << "DIRECCION MEMORIA ["  << &localVariable << RESET << "] "           << GREEN_BG << WHITE << "STACK "  << RESET << " : " << std::setw(10) << localVariable        << " variable local "     << std::endl;
    std::cout << RESET << "DIRECCION MEMORIA ["  << heapVariable << RESET << "]\t   "         << RED_BG << WHITE << "HEAP  "    << RESET << " : " << std::setw(10) << *heapVariable        << " variable en el heap" << std::endl;
    std::cout << RESET << "DIRECCION MEMORIA ["  << &staticLocalVariable << RESET << "]\t   " << BLACK_BG << YELLOW << "STATIC" << RESET << " : " << std::setw(10) << staticLocalVariable  << " variable statica "   << std::endl;

    std::cout << std::endl << "-----------------------------------------------------------------------------\n\n" << RESET << std::boolalpha;
    std::cout << "VERIFICACIONES" << std::endl << std::endl;

    std::cout << RESET << "1. globalVariable está en el heap: "      << (isAddressInHeap(&globalVariable)? GREEN_BG: RED) << isAddressInHeap(&globalVariable)             << RESET << std::endl;
    std::cout << RESET << "2. localVariable está en el heap: "       << (isAddressInHeap(&localVariable)? GREEN_BG: RED) << isAddressInHeap(&localVariable)               << RESET << std::endl;
    std::cout << RESET << "3. heapVariable está en el heap: "        << (isAddressInHeap(heapVariable)? GREEN_BG: RED) << isAddressInHeap(heapVariable)                   << RESET << std::endl;
    std::cout << RESET << "4. staticLocalVariable está en el heap: " << (isAddressInHeap(&staticLocalVariable)? GREEN_BG: RED) << isAddressInHeap(&staticLocalVariable)   << RESET << std::endl << std::endl;

    std::cout << RESET << "5. globalVariable está en la pila: "      << (isAddressInStack(&globalVariable)? GREEN_BG: RED) << isAddressInStack(&globalVariable)           << RESET << std::endl;
    std::cout << RESET << "6. localVariable está en la pila: "       << (isAddressInStack(&localVariable)? GREEN_BG: RED) << isAddressInStack(&localVariable)             << RESET << std::endl;
    std::cout << RESET << "7. heapVariable está en la pila: "        << (isAddressInStack(heapVariable)? GREEN_BG: RED)   << isAddressInStack(heapVariable)               << RESET << std::endl;
    std::cout << RESET << "8. staticLocalVariable está en la pila: " << (isAddressInStack(&staticLocalVariable)? GREEN_BG: RED) << isAddressInStack(&staticLocalVariable) << RESET << std::endl;

    delete heapVariable;

    return 0;
}