#include "Imagen.h"
#include "VisorImagen.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "❌ Uso: " << argv[0] << " <archivo_imagen>" << std::endl;
        return 1;
    }

    std::string archivo = argv[1];

    Imagen imagen(archivo);
    imagen.cargarImagen();
    imagen.extraerCabecera();
    imagen.mostrarCabecera();
    imagen.mostrarDimensiones();

    imagen.guardarMatrizPixeles(); // Guardar matrices por canal

    VisorImagen::mostrar(imagen.obtenerMatriz(), "Imagen Original");

    return 0;
}
