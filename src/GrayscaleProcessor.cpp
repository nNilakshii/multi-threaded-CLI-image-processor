#include "GrayscaleProcessor.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>
#include <vector>

namespace {
    inline std::uint8_t toGray(const Pixel& pixel) {
        const double gray = 0.299 * static_cast<double>(pixel.r) +
                            0.587 * static_cast<double>(pixel.g) +
                            0.114 * static_cast<double>(pixel.b);
        return static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::lround(gray)), 0, 255));
    }

    void grayscaleRange(Pixel* begin, Pixel* end) {
        for (Pixel* current = begin; current != end; ++current) {
            const std::uint8_t gray = toGray(*current);
            current->r = gray;
            current->g = gray;
            current->b = gray;
        }
    }
}

void applyGrayscaleSingle(BMPImage& image) {
    auto& pixels = image.pixels();
    grayscaleRange(pixels.data(), pixels.data() + pixels.size());
}

void applyGrayscaleParallel(BMPImage& image, unsigned int threadCount) {
    const int height = image.height();
    if (height <= 0) {
        return;
    }

    if (threadCount <= 1 || static_cast<unsigned int>(height) == 1u) {
        applyGrayscaleSingle(image);
        return;
    }

    threadCount = std::min<unsigned int>(threadCount, static_cast<unsigned int>(height));

    auto& pixels = image.pixels();
    Pixel* base = pixels.data();
    const int width = image.width();

    const int rowsPerThread = height / static_cast<int>(threadCount);
    int remainder = height % static_cast<int>(threadCount);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    int currentRow = 0;
    for (unsigned int i = 0; i < threadCount; ++i) {
        const int span = rowsPerThread + (remainder > 0 ? 1 : 0);
        if (remainder > 0) {
            --remainder;
        }

        const int startRow = currentRow;
        const int endRow = startRow + span;
        currentRow = endRow;

        Pixel* start = base + static_cast<std::size_t>(startRow) * static_cast<std::size_t>(width);
        Pixel* finish = base + static_cast<std::size_t>(endRow) * static_cast<std::size_t>(width);

        threads.emplace_back([start, finish]() {
            grayscaleRange(start, finish);
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

std::pair<double, double> benchmarkGrayscale(BMPImage& image, unsigned int threadCount) {
    using clock = std::chrono::steady_clock;
    auto singleImage = image;

    const auto singleStart = clock::now();
    applyGrayscaleSingle(singleImage);
    const auto singleEnd = clock::now();
    const double singleMs = std::chrono::duration<double, std::milli>(singleEnd - singleStart).count();

    double multiMs = 0.0;
    if (threadCount > 1) {
        auto multiImage = image;
        const auto multiStart = clock::now();
        applyGrayscaleParallel(multiImage, threadCount);
        const auto multiEnd = clock::now();
        multiMs = std::chrono::duration<double, std::milli>(multiEnd - multiStart).count();
        image = std::move(multiImage);
    } else {
        image = std::move(singleImage);
        return {singleMs, singleMs};
    }

    return {singleMs, multiMs};
}
