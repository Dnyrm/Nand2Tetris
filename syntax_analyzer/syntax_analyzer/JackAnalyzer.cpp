//
// Created by Daniel Madjar on 5/26/24.
//

#include "JackAnalyzer.h"

#include "CompilationEngine.h"
#include "JackTokenizer.h"

JackAnalyzer::JackAnalyzer() {

}

int JackAnalyzer::analyze(std::string path) {
    const std::filesystem::path directoryPath = path;

    if (!std::filesystem::exists(directoryPath)) {
        std::cerr << "Error: Directory does not exist." << std::endl;
        return 1;
    }

    std::vector<std::string> files;
    if (std::filesystem::is_directory(directoryPath)) {
        for (const auto& entry: std::filesystem::directory_iterator(directoryPath)) {
            std::string file = entry.path();
            if (file.find(".jack") != std::string::npos) {
                files.push_back(file);
            }
        }
    } else {
        if (path.find(".jack") != std::string::npos) {
            files.push_back(path);
        }
    }

    if (files.empty()) {
        std::cerr << "Error: No .jack files found in directory." << std::endl;
        return 1;
    }


    CompilationEngine c;
    for (const auto &file : files) {
        JackTokenizer tokenizer(file);

        size_t extensionPos = file.find(".jack");
        std::string outputFileName = file.substr(0, extensionPos) + "C.xml";
        std::ofstream outputFile;
        outputFile.open(outputFileName);

        c.compileClass(tokenizer, outputFile);
    }

    return 0;
}