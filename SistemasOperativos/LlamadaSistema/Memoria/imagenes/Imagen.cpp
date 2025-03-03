/*
  Archivo: Imagen.cpp
  Descripción: Implementación de la clase Imagen para manejar imágenes.
  Funcionalidad: Carga imágenes, extrae cabecera, guarda matrices de píxeles como imágenes y archivos de texto.
*/

#include "Imagen.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>

using namespace std;
using namespace cv;

// Definir códigos ANSI para colores en la terminal Linux
#define RESET   "\033[0m"
#define RED     "\033[31m"  // Rojo para valores en hexadecimal
#define BLUE    "\033[34m"  // Azul para caracteres ASCII

// Constructor de la clase Imagen
Imagen::Imagen(const string& archivo) : nombreArchivo(archivo), ancho(0), alto(0) {}

/*
  Carga la imagen en memoria y almacena su matriz de píxeles.
*/
void Imagen::cargarImagen() {
    matrizPixeles = imread(nombreArchivo, IMREAD_UNCHANGED);
    if (matrizPixeles.empty()) {
        cerr << "❌ Error: No se pudo cargar la imagen " << nombreArchivo << endl;
        exit(1);
    }
    ancho = matrizPixeles.cols;
    alto = matrizPixeles.rows;
    cout << "✅ Imagen cargada exitosamente." << endl;
}

/*
  Extrae los primeros 64 bytes de la cabecera del archivo de imagen.
*/
void Imagen::extraerCabecera() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "❌ Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }
    cabecera.resize(64);
    archivo.read(reinterpret_cast<char*>(cabecera.data()), cabecera.size());
    archivo.close();
    cout << "✅ Cabecera extraída correctamente." << endl;
}

/*
  Muestra la cabecera en formato hexadecimal y ASCII con colores.
*/
void Imagen::mostrarCabecera() const {
    cout << "🔍 **Cabecera de la imagen (" << nombreArchivo << ")**\n";
    
    for (size_t i = 0; i < cabecera.size(); i++) {
        char c = (cabecera[i] >= 32 && cabecera[i] <= 126) ? cabecera[i] : '#';
        //char c = cabecera[i];
             cout << "[" << setw(3) << setfill('0') << static_cast<int>(i) << "]"
             << RED << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(cabecera[i]) << RESET
             << ":"
             << BLUE << c << RESET << "";

        if ((i + 1) % 8 == 0) cout << "";
        if ((i + 1) % 16 == 0) cout << endl;
    }
    
    cout << dec << endl;
}

/*
  Muestra las dimensiones de la imagen en píxeles.
*/
void Imagen::mostrarDimensiones() const {
    cout << "📏 **Dimensiones de la imagen:** " << ancho << "x" << alto << " píxeles." << endl;
}

/*
  Obtiene la matriz de píxeles de la imagen cargada.
*/
cv::Mat Imagen::obtenerMatriz() const {
    return matrizPixeles;
}

/*
  Guarda las matrices de los canales en archivos PNG.
*/
void Imagen::guardarMatrizPixeles() const {
    if (matrizPixeles.empty()) {
        cerr << "❌ No se puede guardar la matriz porque la imagen no se ha cargado." << endl;
        return;
    }

    vector<Mat> canales;
    split(matrizPixeles, canales);

    imwrite("canal_azul.png", canales[0]);
    imwrite("canal_verde.png", canales[1]);
    imwrite("canal_rojo.png", canales[2]);
    imwrite("imagen_completa.png", matrizPixeles);

    cout << "✅ Matrices de píxeles guardadas en archivos PNG." << endl;
}

/*
  Guarda las matrices de los píxeles en archivos de texto con valores numéricos.
*/
void Imagen::guardarMatrizComoTexto() const {
    if (matrizPixeles.empty()) {
        cerr << "❌ No se puede guardar la matriz en texto porque la imagen no se ha cargado." << endl;
        return;
    }

    vector<Mat> canales;
    split(matrizPixeles, canales);

    ofstream archivoAzul("canal_azul.txt"), archivoVerde("canal_verde.txt"), archivoRojo("canal_rojo.txt");
    ofstream archivoCompleto("imagen_completa.txt");

    if (!archivoAzul || !archivoVerde || !archivoRojo || !archivoCompleto) {
        cerr << "❌ Error al crear los archivos de texto." << endl;
        return;
    }

    for (int i = 0; i < matrizPixeles.rows; i++) {
        for (int j = 0; j < matrizPixeles.cols; j++) {
            archivoAzul << setw(3) << (int)canales[0].at<uchar>(i, j) << " ";
            archivoVerde << setw(3) << (int)canales[1].at<uchar>(i, j) << " ";
            archivoRojo << setw(3) << (int)canales[2].at<uchar>(i, j) << " ";

            Vec3b pixel = matrizPixeles.at<Vec3b>(i, j);
            archivoCompleto << "(" << setw(3) << (int)pixel[0] << ","
                            << setw(3) << (int)pixel[1] << ","
                            << setw(3) << (int)pixel[2] << ") ";
        }
        archivoAzul << endl;
        archivoVerde << endl;
        archivoRojo << endl;
        archivoCompleto << endl;
    }

    archivoAzul.close();
    archivoVerde.close();
    archivoRojo.close();
    archivoCompleto.close();

    cout << "✅ Matrices de píxeles guardadas en archivos de texto." << endl;
}
