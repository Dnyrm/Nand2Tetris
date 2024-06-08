//
// Created by Daniel Madjar on 5/25/24.
//

#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H
#if defined __cplusplus

#include <iostream>
#include <fstream>

#include "TokenType.h"
#include "KeyWord.h"

class JackTokenizer {
private:
    static const std::vector<std::string> keywords;
    static const std::vector<char> symbols;
    static const std::vector<KeyWord> keyWordMap;

    std::string inputFileName;
    std::ifstream inputFile;
    std::string token;
    std::string nextToken;
public:
    explicit JackTokenizer(std::string inputFileName);

    void printNextToken();

    void printXML(std::string token, std::ofstream &outputFile);

    std::string getToken();

    bool hasMoreTokens();

    void advance();

    TokenType tokenType(std::string token);

    KeyWord keyWord();

    char symbol();

    std::string identifier();

    int intVal();

    std::string stringVal();
};

#endif
#endif //JACKTOKENIZER_H
