//
// Created by Daniel Madjar on 5/25/24.
//

#include <iostream>
#include <filesystem>
#include <vector>

#include "JackAnalyzer.h"
#include "JackTokenizer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: directory path with .jack files or a single .jack file" << std::endl;
        return 1;
    }

    JackAnalyzer analyzer;
    analyzer.analyze(argv[1]);


    // std::filesystem::path directoryPath = argv[1];
    //
    // if (!std::filesystem::exists(directoryPath)) {
    //     std::cerr << "Error: Directory does not exist." << std::endl;
    //     return 1;
    // }
    //
    // std::vector<std::string> files;
    // if (std::filesystem::is_directory(directoryPath)) {
    //     for (const auto& entry: std::filesystem::directory_iterator(directoryPath)) {
    //         std::string file = entry.path();
    //         if (file.find(".jack") != std::string::npos) {
    //             files.push_back(file);
    //         }
    //     }
    // } else {
    //     std::string file = argv[1];
    //     if (file.find(".jack") != std::string::npos) {
    //         files.push_back(file);
    //     }
    // }
    //
    // if (files.empty()) {
    //     std::cerr << "Error: No .jack files found in directory." << std::endl;
    //     return 1;
    // }


    // for (const auto & file : files) {
    //     JackTokenizer tokenizer(file);
    //
    //     while(tokenizer.hasMoreTokens()) {
    //         tokenizer.advance();
    //
    //         TokenType t = tokenizer.tokenType();
    //
    //     }
    // }

    // std::string str;
    // std::ifstream file;
    // file.open("ArrayTest/Main.jack");
    // while(file >> str){
    //     //Look through the file
    //     if(str == "NEWWORD"){
    //         //Do something with the word following NEWWORD
    //         if (file >> str) {
    //             // the word following NEWWORD is now in str
    //         }
    //     }
    //
    // }
    // file.close();

    return 0;
}