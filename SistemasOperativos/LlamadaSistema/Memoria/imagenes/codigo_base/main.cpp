/// Archivo: main.cpp
/// Programa principal para procesar imágenes y extraer sus canales de color.

#include "imagen.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta_de_la_imagen>" << std::endl;
        return 1;
    }
    
    std::string rutaImagen = argv[1];
    Imagen imagen(rutaImagen);
    
    if (!imagen.cargarImagen()) {
        return 1;
    }
    
    imagen.mostrarCabecera();
    imagen.extraerCanales();
    imagen.guardarCanalesCSV();
    //std::vector<std::vector<int>> canalRojo = imagen.extraerCanal(0);
    //std::vector<std::vector<int>> canalVerde = imagen.extraerCanal(1);
    //std::vector<std::vector<int>> canalAzul = imagen.extraerCanal(2);

    imagen.guardarImagen("imagen_guardada.png");
    
    imagen.mostrarImagenes();
 
    
    return 0;
}
