#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <cstdlib> // Para std::atoi

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tamaño> <nombre_archivo.csv>" << std::endl;
        return 1;
    }

    int tamano = std::atoi(argv[1]);
    std::string nombreArchivo = argv[2];

    if (tamano <= 0) {
        std::cerr << "Error: El tamaño debe ser un entero positivo." << std::endl;
        return 1;
    }

    std::ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << nombreArchivo << "' para escritura." << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100000000); // Números aleatorios entre 1 y 100

    for (int i = 0; i < tamano; ++i) {
        archivo << distrib(gen);
        if (i < tamano - 1) {
            archivo << ","; // Separador de coma
        }
    }

    archivo.close();
    std::cout << "Archivo '" << nombreArchivo << "' con " << tamano << " registros generado exitosamente." << std::endl;

    return 0;
}