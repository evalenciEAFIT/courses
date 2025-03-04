/// @file imagen.cpp
/// @brief Implementación de la clase Imagen para manejar archivos BMP, JPG, PNG, WEBP y GIF.

#include "imagen.h"

/// @brief Constructor de la clase Imagen
/// @param rutaArchivo Ruta del archivo de imagen
Imagen::Imagen(const std::string& rutaArchivo) : ruta(rutaArchivo), ancho(0), alto(0), canales(0), datos(nullptr) {}

/// @brief Destructor para liberar memoria
Imagen::~Imagen() {
    if (datos) {
        stbi_image_free(datos);
    }
}

/// @brief Cargar la imagen y extraer la información
/// @return true si la carga es exitosa, false en caso contrario
bool Imagen::cargarImagen() {
    datos = stbi_load(ruta.c_str(), &ancho, &alto, &canales, 0);
    if (!datos) {
        std::cerr << "Error: No se pudo cargar la imagen " << ruta << std::endl;
        return false;
    }
    return true;
}

/// @brief Mostrar la cabecera de la imagen en consola
void Imagen::mostrarCabecera() const {
    std::cout << "Imagen: " << ruta << std::endl;
    std::cout << "Dimensiones: " << ancho << "x" << alto << std::endl;
    std::cout << "Canales: " << canales << std::endl;
}

/// @brief Extraer los canales RGB a matrices
void Imagen::extraerCanales() {
    canalRojo.resize(alto, std::vector<int>(ancho));
    canalVerde.resize(alto, std::vector<int>(ancho));
    canalAzul.resize(alto, std::vector<int>(ancho));
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int index = (y * ancho + x) * canales;
            canalRojo[y][x] = datos[index];
            canalVerde[y][x] = datos[index + 1];
            canalAzul[y][x] = datos[index + 2];
        }
    }
}

/// @brief Extraer un canal específico (0: rojo, 1: verde, 2: azul)
std::vector<std::vector<int>> Imagen::extraerCanal(int canal) const {
    std::vector<std::vector<int>> canalExtraido(alto, std::vector<int>(ancho));
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int index = (y * ancho + x) * canales;
            canalExtraido[y][x] = datos[index + canal];
        }
    }
    return canalExtraido;
}

/// @brief Guardar los canales en archivos CSV
void Imagen::guardarCanalesCSV() const {
    std::ofstream archivoRojo("canal_rojo.csv");
    std::ofstream archivoVerde("canal_verde.csv");
    std::ofstream archivoAzul("canal_azul.csv");
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            archivoRojo << canalRojo[y][x] << (x < ancho - 1 ? "," : "\n");
            archivoVerde << canalVerde[y][x] << (x < ancho - 1 ? "," : "\n");
            archivoAzul << canalAzul[y][x] << (x < ancho - 1 ? "," : "\n");
        }
    }
    archivoRojo.close();
    archivoVerde.close();
    archivoAzul.close();
    
    std::cout << "Canales guardados en archivos CSV.\n";
}

/// @brief Mostrar la imagen y sus canales con OpenCV
void Imagen::mostrarImagenes() const {
    cv::Mat imagenOriginal(alto, ancho, CV_8UC3);
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int index = (y * ancho + x) * canales;
            imagenOriginal.at<cv::Vec3b>(y, x) = cv::Vec3b(datos[index + 2], datos[index + 1], datos[index]);
        }
    }
    
    cv::Mat rojo(alto, ancho, CV_8UC1);
    cv::Mat verde(alto, ancho, CV_8UC1);
    cv::Mat azul(alto, ancho, CV_8UC1);
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            rojo.at<uchar>(y, x) = canalRojo[y][x];
            verde.at<uchar>(y, x) = canalVerde[y][x];
            azul.at<uchar>(y, x) = canalAzul[y][x];
        }
    }
    
    cv::imshow("Imagen Original", imagenOriginal);
    cv::imshow("Canal Rojo", rojo);
    cv::imshow("Canal Verde", verde);
    cv::imshow("Canal Azul", azul);
    
    // Espera hasta que se cierren todas las ventanas
    while (cv::waitKey(1) != 27) {} // Presionar ESC para salir
    cv::destroyAllWindows();
}

/// @brief Guardar la imagen en formato PNG
void Imagen::guardarImagen(const std::string& nombreArchivo) const {
    cv::Mat imagenOriginal(alto, ancho, CV_8UC3);
    
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int index = (y * ancho + x) * canales;
            imagenOriginal.at<cv::Vec3b>(y, x) = cv::Vec3b(datos[index + 2], datos[index + 1], datos[index]);
        }
    }
    
    cv::imwrite(nombreArchivo, imagenOriginal);
}
