#ifndef GRAYSCALEPROCESSOR_H
#define GRAYSCALEPROCESSOR_H

#include "BMPImage.h"

#include <utility>

void applyGrayscaleSingle(BMPImage& image);
void applyGrayscaleParallel(BMPImage& image, unsigned int threadCount);
std::pair<double, double> benchmarkGrayscale(BMPImage& image, unsigned int threadCount);

#endif // GRAYSCALEPROCESSOR_H
