#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip> // Para formatear la salida

// Función para fusionar dos subarreglos ordenados
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Crear arreglos temporales
    std::vector<int> L(n1), R(n2);

    // Copiar datos a los arreglos temporales
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    // Fusionar los arreglos temporales de vuelta al arreglo original
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) arr[k++] = L[i++];

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) arr[k++] = R[j++];
}

// Función principal del Merge Sort con concurrencia
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if(right - left > 1000){ // Si el tamaño del vector es suficientemente grande para que valga la pena el paralelismo
            std::thread t1(parallelMergeSort, std::ref(arr), left, mid);
            std::thread t2(parallelMergeSort, std::ref(arr), mid + 1, right);
            t1.join();
            t2.join();
        } else {
            parallelMergeSort(arr, left, mid);
            parallelMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// Función para el Merge Sort secuencial
void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

std::vector<int> readDataFromCSV(const std::string& filename) {
    std::vector<int> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            try {
                data.push_back(std::stoi(value));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Advertencia: valor no numérico en el archivo CSV." << std::endl;
            }
        }
    }
    file.close();
    return data;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <nombre_archivo.csv>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<int> data = readDataFromCSV(filename);

    if (data.empty()) {
        return 1;
    }

    std::vector<int> dataParallel = data; // Copia para el ordenamiento paralelo
    std::vector<int> dataSequential = data; // Copia para el ordenamiento secuencial

    // Ordenamiento paralelo
    auto parallelStart = std::chrono::high_resolution_clock::now();
    parallelMergeSort(dataParallel, 0, dataParallel.size() - 1);
    auto parallelEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallelDuration = parallelEnd - parallelStart;

    // Ordenamiento secuencial
    auto sequentialStart = std::chrono::high_resolution_clock::now();
    sequentialMergeSort(dataSequential, 0, dataSequential.size() - 1);
    auto sequentialEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sequentialDuration = sequentialEnd - sequentialStart;

    // Resultados
    std::cout << "Resultados de ordenamiento paralelo:\n";
    std::cout << "Tiempo de ejecución (paralelo): " << std::fixed << std::setprecision(6) << parallelDuration.count() << " segundos\n";
    std::cout << "Primeros 10 elementos: ";
    for (int i = 0; i < std::min((int)dataParallel.size(), 10); ++i) std::cout << dataParallel[i] << " ";
    std::cout << "\nÚltimos 10 elementos: ";
    for (int i = std::max(0, (int)dataParallel.size() - 10); i < dataParallel.size(); ++i) std::cout << dataParallel[i] << " ";

    std::cout << "\n\nResultados de ordenamiento secuencial:\n";
    std::cout << "Tiempo de ejecución (secuencial): " << std::fixed << std::setprecision(6) << sequentialDuration.count() << " segundos\n";
    std::cout << "Primeros 10 elementos: ";
    for (int i = 0; i < std::min((int)dataSequential.size(), 10); ++i) std::cout << dataSequential[i] << " ";
    std::cout << "\nÚltimos 10 elementos: ";
    for (int i = std::max(0, (int)dataSequential.size() - 10); i < dataSequential.size(); ++i) std::cout << dataSequential[i] << " ";
    std::cout << std::endl;

    return 0;
}