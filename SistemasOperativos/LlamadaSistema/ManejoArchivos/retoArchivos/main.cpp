#include "ManejadorArchivo.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada> [archivo_salida]" << std::endl;
        return 1;
    }

    std::string archivoEntrada = argv[1];
    std::string archivoSalida = (argc > 2) ? argv[2] : "salida.bin";

    ManejadorArchivo manejador;

    if (manejador.leerArchivo(archivoEntrada)) {
        manejador.convertirABits();
        manejador.mostrarBits();
        manejador.escribirArchivo(archivoSalida);

        std::vector<std::vector<int>> matriz = manejador.matrizBits();
        std::cout << "\nMatriz de bits del archivo:\n";
        int tamano = matriz.size();
        int indice = 0;
        std::cout << "  Tamaño lista:" << tamano << std::endl;
        for (const auto& letra : matriz) {
            std::cout << "[" << indice << "/" << tamano-1 << "] ";
            for (int bit : letra) 
                std::cout<< bit;
            indice++;
            
            std::cout << std::endl;
        }
    }

    return 0;
}
