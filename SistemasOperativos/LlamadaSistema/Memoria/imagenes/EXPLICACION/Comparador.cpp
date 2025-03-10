// Activar la implementación de stb_image y stb_image_write
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Comparador.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include <iostream>
#include <chrono> // Para medir tiempo
#include <sys/resource.h> // Para medir consumo de memoria
#include <sys/time.h>

using namespace std;
using namespace std::chrono;

// Constructor
Comparador::Comparador() : ancho(0), alto(0) {}

// Destructor
Comparador::~Comparador() {}

// Cargar una imagen desde archivo usando stb_image
bool Comparador::cargarDesdeArchivo(const string &archivoEntrada) {
    int nCanales;

    // Cargamos la imagen y obtenemos sus dimensiones
    unsigned char *data = stbi_load(archivoEntrada.c_str(), &ancho, &alto, &nCanales, 3);
    if (!data) {
        cerr << "❌ Error: No se pudo cargar la imagen '" << archivoEntrada << "'.\n";
        return false;
    }

    // Ajustamos las matrices para las dimensiones de la imagen
    pixelesChar.resize(alto, vector<PixelChar>(ancho));
    pixelesInt.resize(alto, vector<PixelInt>(ancho));

    // Guardamos los datos en ambas matrices
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            int index = (y * ancho + x) * 3;
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];

            // Asignamos los valores a las dos matrices
            pixelesChar[y][x] = {r, g, b};
            pixelesInt[y][x] = {r, g, b};
        }
    }

    stbi_image_free(data);

    cout << "✅ Imagen cargada correctamente desde '" << archivoEntrada << "'\n";
    return true;
}

// Guardar la imagen en archivo usando stb_image_write
bool Comparador::guardarEnArchivo(const string &archivoSalida) {
    vector<unsigned char> buffer(ancho * alto * 3);

    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            int index = (y * ancho + x) * 3;
            buffer[index] = pixelesChar[y][x].r;
            buffer[index + 1] = pixelesChar[y][x].g;
            buffer[index + 2] = pixelesChar[y][x].b;
        }
    }

    if (stbi_write_png(archivoSalida.c_str(), ancho, alto, 3, buffer.data(), ancho * 3)) {
        cout << "✅ Imagen guardada en '" << archivoSalida << "'\n";
        return true;
    } else {
        cerr << "❌ Error al guardar la imagen.\n";
        return false;
    }
}

// Invertir colores usando char
void Comparador::invertirColoresChar() {
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            pixelesChar[y][x].r = 255 - pixelesChar[y][x].r;
            pixelesChar[y][x].g = 255 - pixelesChar[y][x].g;
            pixelesChar[y][x].b = 255 - pixelesChar[y][x].b;
        }
    }
}

// Invertir colores usando int
void Comparador::invertirColoresInt() {
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            pixelesInt[y][x].r = 255 - pixelesInt[y][x].r;
            pixelesInt[y][x].g = 255 - pixelesInt[y][x].g;
            pixelesInt[y][x].b = 255 - pixelesInt[y][x].b;
        }
    }
}

// Medir consumo de memoria usando sizeof()
size_t Comparador::calcularMemoriaChar() {
    return alto * ancho * sizeof(PixelChar);
}

size_t Comparador::calcularMemoriaInt() {
    return alto * ancho * sizeof(PixelInt);
}

// Medir tiempo y consumo de memoria
void Comparador::medirTiempoYMemoria() {
    auto inicio = high_resolution_clock::now();
    invertirColoresChar();
    auto fin = high_resolution_clock::now();
    auto duracionChar = duration_cast<microseconds>(fin - inicio).count();

    inicio = high_resolution_clock::now();
    invertirColoresInt();
    fin = high_resolution_clock::now();
    auto duracionInt = duration_cast<microseconds>(fin - inicio).count();

    cout << "\n[ESTADÍSTICAS DE COMPARACIÓN]\n";
    cout << "Tiempo usando char: " << duracionChar << " microsegundos\n";
    cout << "Tiempo usando int: " << duracionInt << " microsegundos\n";
    cout << "Memoria usando char: " << calcularMemoriaChar() / 1024 << " KB\n";
    cout << "Memoria usando int: " << calcularMemoriaInt() / 1024 << " KB\n";
}
