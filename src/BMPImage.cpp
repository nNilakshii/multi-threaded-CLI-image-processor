#include "BMPImage.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace {
    std::size_t computeRowStride(int width) {
        const std::size_t bytesPerRow = static_cast<std::size_t>(width) * 3;
        return (bytesPerRow + 3) & ~static_cast<std::size_t>(3);
    }
}

void BMPImage::load(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("failed to open input BMP: " + path);
    }

    input.read(reinterpret_cast<char*>(&fileHeader_), sizeof(fileHeader_));
    input.read(reinterpret_cast<char*>(&infoHeader_), sizeof(infoHeader_));
    if (!input) {
        throw std::runtime_error("failed to read BMP headers from: " + path);
    }

    if (fileHeader_.bfType != expectedMagic) {
        throw std::runtime_error("unsupported BMP format: invalid magic");
    }

    if (infoHeader_.biBitCount != expectedBitCount) {
        throw std::runtime_error("unsupported BMP format: only 24-bit BMP supported");
    }

    if (infoHeader_.biCompression != compressionNone) {
        throw std::runtime_error("unsupported BMP format: compression not supported");
    }

    width_ = infoHeader_.biWidth;
    height_ = std::abs(infoHeader_.biHeight);
    topDown_ = infoHeader_.biHeight < 0;

    if (width_ <= 0 || height_ <= 0) {
        throw std::runtime_error("unsupported BMP format: invalid dimensions");
    }

    const std::size_t rowStride = computeRowStride(width_);
    pixels_.assign(static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_), {});

    input.seekg(static_cast<std::streamoff>(fileHeader_.bfOffBits), std::ios::beg);
    if (!input) {
        throw std::runtime_error("failed to seek to pixel data");
    }

    std::vector<std::uint8_t> row(rowStride);
    for (int rowIndex = 0; rowIndex < height_; ++rowIndex) {
        input.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(rowStride));
        if (!input) {
            throw std::runtime_error("unexpected EOF while reading pixel data");
        }

        const int targetRow = topDown_ ? rowIndex : (height_ - 1 - rowIndex);
        Pixel* pixelRow = pixels_.data() + static_cast<std::size_t>(targetRow) * static_cast<std::size_t>(width_);
        for (int col = 0; col < width_; ++col) {
            const std::size_t base = static_cast<std::size_t>(col) * 3;
            pixelRow[col].b = row[base + 0];
            pixelRow[col].g = row[base + 1];
            pixelRow[col].r = row[base + 2];
        }
    }
}

void BMPImage::save(const std::string& path) const {
    if (pixels_.empty()) {
        throw std::runtime_error("no pixel data to save");
    }

    BMPFileHeader fileHeader = fileHeader_;
    BMPInfoHeader infoHeader = infoHeader_;

    infoHeader.biSizeImage = static_cast<std::uint32_t>(computeRowStride(width_) * static_cast<std::size_t>(height_));
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("failed to open output BMP: " + path);
    }

    output.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    output.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    if (!output) {
        throw std::runtime_error("failed to write BMP headers to: " + path);
    }

    const std::size_t rowStride = computeRowStride(width_);
    std::vector<std::uint8_t> row(rowStride, 0);
    for (int rowIndex = 0; rowIndex < height_; ++rowIndex) {
        const int sourceRow = topDown_ ? rowIndex : (height_ - 1 - rowIndex);
        const Pixel* pixelRow = pixels_.data() + static_cast<std::size_t>(sourceRow) * static_cast<std::size_t>(width_);
        for (int col = 0; col < width_; ++col) {
            const std::size_t base = static_cast<std::size_t>(col) * 3;
            row[base + 0] = pixelRow[col].b;
            row[base + 1] = pixelRow[col].g;
            row[base + 2] = pixelRow[col].r;
        }
        output.write(reinterpret_cast<const char*>(row.data()), static_cast<std::streamsize>(rowStride));
        if (!output) {
            throw std::runtime_error("failed to write pixel data to: " + path);
        }
    }
}

void BMPImage::applyGrayscale() {
    for (auto& pixel : pixels_) {
        const double gray = 0.299 * static_cast<double>(pixel.r) +
                            0.587 * static_cast<double>(pixel.g) +
                            0.114 * static_cast<double>(pixel.b);
        const auto clamped = static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::lround(gray)), 0, 255));
        pixel.r = clamped;
        pixel.g = clamped;
        pixel.b = clamped;
    }
}
