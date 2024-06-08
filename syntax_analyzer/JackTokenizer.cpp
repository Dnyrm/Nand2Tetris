//
// Created by Daniel Madjar on 5/25/24.
//

#include "JackTokenizer.h"

const std::vector<std::string> JackTokenizer::keywords = {
    "class", "constructor", "function", "method", "field", "static", "var", "int", "char",
    "boolean", "void", "true","false", "null", "this", "let", "do", "if",
    "else", "while", "return"
};

const std::vector<char> JackTokenizer::symbols = {
    '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
};

const std::vector<KeyWord> JackTokenizer::keyWordMap = {
    CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET,
    DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, K_NULL, THIS
};

std::string& trim(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

bool isBlankOrComment(std::string line) {
    if (line.empty()) {
        return true;
    }

    if (line.length() >= 3) {
        std::string multiLineComment = line.substr(0, 3);
        if (multiLineComment == "/**") {
            return true;
        }
    }

    if (line.length() >= 2) {
        std::string singleLineComment = line.substr(0, 2);
        if (singleLineComment == "//" || singleLineComment == "/*") {
            return true;
        }
    }

    return false;
}

std::string JackTokenizer::getToken() {
    return this->token;
}

JackTokenizer::JackTokenizer(std::string inputFileName) {
    this->inputFileName = inputFileName;
    this->inputFile.open(inputFileName);
}

void JackTokenizer::printXML(std::string token, std::ofstream &outputFile) {
    TokenType type = this->tokenType(token);

    if (type == TokenType::KEYWORD) {
        outputFile << "<keyword> " + token + " </keyword>" << std::endl;
    } else if (type == TokenType::SYMBOL) {
        outputFile << "<symbol> " + token + " </symbol>" << std::endl;
    } else if (type == TokenType::IDENTIFIER) {
        outputFile << "<identifier> " + token + " </identifier>" << std::endl;
    } else if (type == TokenType::INT_CONST) {
        outputFile << "<integerConstant> " + token + " </integerConstant>" << std::endl;
    } else {
        outputFile << "<stringConstant> " + token.substr(1, token.length() - 2) + " </stringConstant>" << std::endl;
    }
}

bool JackTokenizer::hasMoreTokens() {
    return this->inputFile.peek() != EOF;
}

void JackTokenizer::advance() {
    std::string line;
    if (this->nextToken.empty()) {
        getline(this->inputFile, line);
    } else {
        line = this->nextToken;
    }

    trim(line);

    if (line[0] == '\"') {
        size_t pos = line.find_last_of('\"');
        this->token = line.substr(0, pos + 1);
        this->nextToken = line.substr(pos + 1, line.length() - pos + 1);
        return;
    }

    while (isBlankOrComment(line)) {
        if (!hasMoreTokens()) {
            this->token = "";
            this->nextToken = "";
            return;
        }

        if (line.empty()) {
            getline(this->inputFile, line);
            trim(line);
            continue;
        }

        if (line.length() >= 3) {
            std::string multiLineComment = line.substr(0, 3);
            if (multiLineComment == "/**") {
                size_t pos = line.find("*/");
                while (pos == std::string::npos) {
                    getline(this->inputFile, line);
                    pos = line.find("*/");
                }

                size_t len = line.length() - 2;
                if (pos == len) {
                    getline(this->inputFile, line);
                    trim(line);
                    continue;
                } else {
                    line = line.substr(pos + 2, line.length() - pos + 2);
                    trim(line);
                    continue;
                }
            }
        }

        if (line.length() >= 2) {
            std::string singleLineComment = line.substr(0, 2);
            if (singleLineComment == "/*") {
                size_t pos = line.find("*/");
                if (pos != std::string::npos) { // */ was found
                    line = line.substr(pos + 2, line.length() - pos + 2);
                    trim(line);
                } else {
                    std::cerr << "Compiler Error: Use of single line comment as a multi-line comment." << std::endl;
                    return;
                }
            } else if (singleLineComment == "//") {
                getline(this->inputFile, line);
                trim(line);
            }
        }
    }

    size_t pos = line.find_first_of(" ,;(){}.[]-~/\"");
    if (pos == std::string::npos) {
        this->token = line;
        this->nextToken = "";
    } else if (pos == 0) {
        this->token = line.substr(0, 1);
        this->nextToken = line.substr(1, line.length() - 1);
    } else {
        this->token = line.substr(0, pos);
        this->nextToken = line.substr(pos, line.length() - pos + 1);
    }
}

TokenType JackTokenizer::tokenType(std::string token) {
    for (const auto &keyword: this->keywords) {
        if (token == keyword) {
            return TokenType::KEYWORD;
        }
    }

    for (const auto &symbol: this->symbols) {
        if (token[0] == symbol) {
            return TokenType::SYMBOL;
        }
    }

    if (std::all_of(token.begin(), token.end(), ::isdigit)) {
        return TokenType::INT_CONST;
    }

    size_t len = token.length();
    if (len > 0) {
        const char first = token[0];
        const char last = token[len - 1];

        if (first == '"' && last == '"') {
            return TokenType::STRING_CONST;
        }
    }

    return TokenType::IDENTIFIER;
}

KeyWord JackTokenizer::keyWord() {
    // Only called when this->token is KEYWORD
    auto it = std::find(this->keywords.begin(), this->keywords.end(), this->token);
    if (it != this->keywords.end()) {
        size_t index = it - this->keywords.begin();
        return this->keyWordMap[index];
    }
}

char JackTokenizer::symbol() {
    return this->token[0];
}

std::string JackTokenizer::identifier() {
    return this->token;
}

int JackTokenizer::intVal() {
    return std::stoi(this->token);
}

std::string JackTokenizer::stringVal() {
    return this->token.substr(1, this->token.length() - 2);
}