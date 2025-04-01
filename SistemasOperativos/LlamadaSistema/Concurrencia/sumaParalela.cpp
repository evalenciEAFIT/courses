#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx; // Mutex para sincronización

// Función para calcular la suma parcial de un rango del vector
void calculatePartialSum(const std::vector<int>& data, int start, int end, int& partialSum) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += data[i];
    }
    std::lock_guard<std::mutex> lock(mtx); // Bloquear el mutex
    partialSum += sum;
}

int main() {
    // Definir el vector de números
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100};

    int numThreads = 2; // Número de hilos
    int chunkSize = data.size() / numThreads; // Tamaño de cada subconjunto
    std::vector<std::thread> threads; // Vector para almacenar los hilos
    int totalSum = 0; // Variable para almacenar la suma total

    // Crear hilos para calcular las sumas parciales
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (start + chunkSize);
        threads.emplace_back(calculatePartialSum, std::ref(data), start, end, std::ref(totalSum));
    }

    // Esperar a que todos los hilos terminen
    for (auto& t : threads) {
        t.join();
    }

    // Mostrar el resultado
    std::cout << "Suma total (versión paralela): " << totalSum << std::endl;

    return 0;
}
