#ifndef VISORIMAGEN_H
#define VISORIMAGEN_H

#include <opencv2/opencv.hpp>
#include <iostream>

class VisorImagen {
public:
    static void mostrar(const cv::Mat& imagen, const std::string& titulo);
};

#endif
