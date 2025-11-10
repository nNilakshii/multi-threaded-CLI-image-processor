#ifndef CLIOPTIONS_H
#define CLIOPTIONS_H

#include <string>
#include <optional>

struct CLIOptions {
    std::string inputPath;
    std::string outputPath;
    std::optional<unsigned int> threadCount;
};

CLIOptions parseArguments(int argc, char* argv[]);

#endif // CLIOPTIONS_H
