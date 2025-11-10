#include "CLIOptions.h"
#include "BMPImage.h"
#include "GrayscaleProcessor.h"

#include <iostream>
#include <thread>
#include <iomanip>

int main(int argc, char* argv[]) {
    try {
        CLIOptions options = parseArguments(argc, argv);

        if (!options.threadCount.has_value()) {
            unsigned int detected = std::thread::hardware_concurrency();
            if (detected == 0) {
                detected = 1; // portable fallback when detection fails
            }
            options.threadCount = detected;
        }

        const unsigned int threadCount = options.threadCount.value();

        std::cout << "Input file:  " << options.inputPath << '\n';
        std::cout << "Output file: " << options.outputPath << '\n';
        std::cout << "Threads:     " << threadCount << '\n';

        BMPImage image;
        image.load(options.inputPath);

        const auto [singleMs, multiMs] = benchmarkGrayscale(image, threadCount);

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Single-thread: " << singleMs << " ms\n";
        if (threadCount > 1) {
            std::cout << threadCount << "-thread:  " << multiMs << " ms\n";
            if (multiMs > 0.0) {
                const double speedup = singleMs / multiMs;
                std::cout << "Speedup:      x" << speedup << '\n';
            }
        }

        image.save(options.outputPath);

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
