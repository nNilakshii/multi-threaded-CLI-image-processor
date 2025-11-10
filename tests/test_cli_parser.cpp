#include "CLIOptions.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    std::vector<char*> toArgv(std::vector<std::string>& storage) {
        std::vector<char*> argv;
        argv.reserve(storage.size());
        for (auto& token : storage) {
            argv.push_back(token.data());
        }
        return argv;
    }

    bool testValidWithThreads() {
        std::vector<std::string> args = {
            "image_processor", "-i", "input.bmp", "-o", "output.bmp", "-t", "4"
        };
        auto argv = toArgv(args);
        CLIOptions options = parseArguments(static_cast<int>(argv.size()), argv.data());
        return options.inputPath == "input.bmp" &&
               options.outputPath == "output.bmp" &&
               options.threadCount.has_value() &&
               options.threadCount.value() == 4u;
    }

    bool testValidWithoutThreads() {
        std::vector<std::string> args = {
            "image_processor", "-i", "in.bmp", "-o", "out.bmp"
        };
        auto argv = toArgv(args);
        CLIOptions options = parseArguments(static_cast<int>(argv.size()), argv.data());
        return options.inputPath == "in.bmp" &&
               options.outputPath == "out.bmp" &&
               !options.threadCount.has_value();
    }

    bool testZeroThreads() {
        std::vector<std::string> args = {
            "image_processor", "-i", "in.bmp", "-o", "out.bmp", "-t", "0"
        };
        auto argv = toArgv(args);
        try {
            (void)parseArguments(static_cast<int>(argv.size()), argv.data());
            return false;
        } catch (const std::invalid_argument&) {
            return true;
        }
    }

    bool testMissingOutput() {
        std::vector<std::string> args = {
            "image_processor", "-i", "in.bmp"
        };
        auto argv = toArgv(args);
        try {
            (void)parseArguments(static_cast<int>(argv.size()), argv.data());
            return false;
        } catch (const std::invalid_argument&) {
            return true;
        }
    }
}

int main() {
    int failures = 0;

    if (!testValidWithThreads()) {
        std::cerr << "testValidWithThreads failed\n";
        ++failures;
    }

    if (!testValidWithoutThreads()) {
        std::cerr << "testValidWithoutThreads failed\n";
        ++failures;
    }

    if (!testZeroThreads()) {
        std::cerr << "testZeroThreads failed\n";
        ++failures;
    }

    if (!testMissingOutput()) {
        std::cerr << "testMissingOutput failed\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All CLI parser tests passed\n";
    }

    return failures == 0 ? 0 : 1;
}
