#include "VisorImagen.h"

using namespace cv;
using namespace std;

/**
 * Muestra la imagen en una ventana de OpenCV.
 */
void VisorImagen::mostrar(const cv::Mat& imagen, const std::string& titulo) {
    if (imagen.empty()) {
        cerr << "❌ Error: No hay datos en la imagen." << endl;
        return;
    }
    
    namedWindow(titulo, WINDOW_AUTOSIZE);
    imshow(titulo, imagen);
    waitKey(0);
}
