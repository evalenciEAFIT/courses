/// Archivo: imagen.h
/// Definición de la clase Imagen para manejar archivos BMP, JPG, PNG, WEBP y GIF.

#ifndef IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h"
#include "stb_image_write.h"
#include <opencv2/opencv.hpp>
#include <fstream>

/// Clase: Imagen
/// metodo: Clase para manejar imágenes y extraer su información
class Imagen {
private:
    std::string ruta;  ///< Ruta de la imagen
    int ancho, alto, canales; ///< Dimensiones y número de canales de la imagen
    unsigned char* datos; ///< Puntero a los datos de la imagen
    std::vector<std::vector<int>> canalRojo, canalVerde, canalAzul; ///< Matrices de los canales RGB

public:
    /// metodo: Constructor de la clase Imagen
    /// parametro entrada: rutaArchivo Ruta del archivo de imagen
    Imagen(const std::string& rutaArchivo);
    
    /// metodo: Destructor para liberar memoria
    ~Imagen();
    
    /// metodo: Cargar la imagen y extraer la información
    bool cargarImagen();
    
    /// metodo: Mostrar la cabecera de la imagen en consola
    void mostrarCabecera() const;
    
    /// metodo: Extraer los canales RGB a matrices
    void extraerCanales();
    std::vector<std::vector<int>> extraerCanal(int canal) const;
    
    /// metodo: Guardar los canales en archivos CSV
    void guardarCanalesCSV() const;
    
    /// metodo: Mostrar la imagen y sus canales con OpenCV
    void mostrarImagenes() const;
    
    /// metodo: Guardar la imagen en formato PNG
    void guardarImagen(const std::string& nombreArchivo) const;
};

#endif
