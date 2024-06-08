//
// Created by Daniel Madjar on 5/26/24.
//

#ifndef JACKANALYZER_H
#define JACKANALYZER_H

#include <iostream>
#include <filesystem>
#include <fstream>

class JackAnalyzer {
private:
public:
    JackAnalyzer();

    int analyze(std::string path);
};

#endif //JACKANALYZER_H
