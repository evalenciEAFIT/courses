#ifndef COMPARADOR_H
#define COMPARADOR_H

#include <vector>
#include <string>
#include <cstddef> // Para size_t

// Estructura para píxeles usando char (más eficiente)
struct PixelChar {
    unsigned char r, g, b;
};

// Estructura para píxeles usando int (más precisa pero más pesada)
struct PixelInt {
    int r, g, b;
};

// Clase Comparador para manejar imágenes
class Comparador {
public:
    Comparador();  // Constructor
    ~Comparador(); // Destructor

    // Funciones para cargar y guardar imágenes
    bool cargarDesdeArchivo(const std::string &archivoEntrada);
    bool guardarEnArchivo(const std::string &archivoSalida);

    // Función para medir tiempo y memoria
    void medirTiempoYMemoria();

    // Mostrar información de la imagen
    void mostrarInformacion() const;

    // Funciones para medir el consumo real de memoria (Declaradas correctamente)
    size_t calcularMemoriaChar();
    size_t calcularMemoriaInt();

private:
    int ancho, alto;

    // Matrices para comparar int vs char
    std::vector<std::vector<PixelChar>> pixelesChar;
    std::vector<std::vector<PixelInt>> pixelesInt;

    // Invertir colores
    void invertirColoresChar();
    void invertirColoresInt();
};

#endif
