//
// Created by Daniel Madjar on 5/26/24.
//

#ifndef COMPILATIONENGINE_H
#define COMPILATIONENGINE_H

#include <iostream>
#include <fstream>

#include "JackTokenizer.h"
#include "TokenState.h"

class CompilationEngine {
private:
    static std::string operators;
    TokenState state;
public:
    CompilationEngine();

    void compileClass(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileStatements(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileClassVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileSubroutine(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileSubroutineBody(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileParameterList(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileDo(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileLet(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileWhile(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileReturn(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileIf(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileExpression(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileTerm(JackTokenizer &tokenizer, std::ofstream &outputFile);

    void compileExpressionList(JackTokenizer &tokenizer, std::ofstream &outputFile);
};

#endif //COMPILATIONENGINE_H
