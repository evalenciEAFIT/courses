#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>

std::mutex mtx;

void calculatePartialSum(const std::vector<int>& data, int start, int end, int& partialSum) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += data[i];
    }
    std::lock_guard<std::mutex> lock(mtx);
    partialSum += sum;
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

    std::string filename = argv[1]; // Obtener el nombre del archivo desde el argumento

    std::vector<int> data = readDataFromCSV(filename);

    if (data.empty()) {
        return 1;
    }

    // Suma paralela
    int numThreads = std::thread::hardware_concurrency();
    int chunkSize = data.size() / numThreads;
    std::vector<std::thread> threads;
    int parallelSum = 0;

    auto parallelStart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (start + chunkSize);
        threads.emplace_back(calculatePartialSum, std::ref(data), start, end, std::ref(parallelSum));
    }

    for (auto& t : threads) {
        t.join();
    }

    auto parallelEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallelDuration = parallelEnd - parallelStart;

    // Suma secuencial
    auto sequentialStart = std::chrono::high_resolution_clock::now();

    int sequentialSum = 0;
    for (int value : data) {
        sequentialSum += value;
    }

    auto sequentialEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sequentialDuration = sequentialEnd - sequentialStart;

    // Resultados
    std::cout << "Suma total (paralela): " << parallelSum << std::endl;
    std::cout << "Tiempo de ejecución (paralelo): " << parallelDuration.count() << " segundos" << std::endl;

    std::cout << "Suma total (secuencial): " << sequentialSum << std::endl;
    std::cout << "Tiempo de ejecución (secuencial): " << sequentialDuration.count() << " segundos" << std::endl;

    return 0;
}