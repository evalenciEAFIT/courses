#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

// Función para simular uso de memoria y rastrear asignaciones y liberaciones.
void rastrearMemoria() {
    std::cout << "Iniciando rastreo de memoria...\n";
    
    // Medir tiempo de ejecución
    auto inicio = std::chrono::high_resolution_clock::now();
    
    // Asignación de memoria con punteros inteligentes
    std::unique_ptr<int[]> buffer(new int[1000000]); // 1 millón de enteros
    
    // Obtener dirección de memoria asignada
    std::cout << "Memoria asignada en el rango: inicio:[" << static_cast<void*>(&buffer[0])
              << "] - - - [" << static_cast<void*>(&buffer[999999]) << "]\n";
    
    // Simulación de uso de memoria
    for (int i = 0; i < 1000000; i++) {
        buffer[i] = i % 255;
    }
    
    // Medir memoria usada antes de liberar
    std::cout << "Memoria asignada y utilizada. Presione ENTER para liberar..." << std::endl;
    std::cin.get();
    
    // `buffer` se libera automáticamente al salir del alcance
    
    // Medir tiempo después de liberación
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;
    
    std::cout << "Memoria liberada. Tiempo de ejecución: " << duracion.count() << " segundos." << std::endl;
}

int main() {
    std::cout << "Ejemplo de Memory Trace en C++\n";
    rastrearMemoria();
    return 0;
}