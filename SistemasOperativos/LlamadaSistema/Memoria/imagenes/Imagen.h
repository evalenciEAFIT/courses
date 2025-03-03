#ifndef IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Imagen {
private:
    std::string nombreArchivo;
    cv::Mat matrizPixeles;
    std::vector<unsigned char> cabecera;
    int ancho;
    int alto;

public:
    Imagen(const std::string& archivo);
    void extraerCabecera();
    void mostrarCabecera() const;
    void cargarImagen();
    void mostrarDimensiones() const;
    cv::Mat obtenerMatriz() const;
    void guardarMatrizPixeles() const;
    void guardarMatrizComoTexto() const;
};

#endif
