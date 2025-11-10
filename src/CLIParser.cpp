#include "CLIOptions.h"

#include <stdexcept>
#include <string>
#include <sstream>

namespace {
    [[noreturn]] void throwUsage(const std::string& message) {
        std::ostringstream oss;
        oss << "Error: " << message << "\n"
            << "Usage: ./image_processor -i <input.bmp> -o <output.bmp> [-t <threads>]";
        throw std::invalid_argument(oss.str());
    }
}

CLIOptions parseArguments(int argc, char* argv[]) {
    if (argc < 5) {
        throwUsage("not enough arguments");
    }

    CLIOptions options;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            options.inputPath = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            options.outputPath = argv[++i];
        } else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            const std::string value = argv[++i];
            try {
                unsigned long parsed = std::stoul(value);
                if (parsed == 0) {
                    throwUsage("thread count must be greater than zero");
                }
                options.threadCount = static_cast<unsigned int>(parsed);
            } catch (const std::exception&) {
                throwUsage("invalid thread count supplied to -t/--threads");
            }
        } else {
            throwUsage("unrecognized or misplaced argument '" + arg + "'");
        }
    }

    if (options.inputPath.empty()) {
        throwUsage("missing required -i/--input option");
    }

    if (options.outputPath.empty()) {
        throwUsage("missing required -o/--output option");
    }

    return options;
}
