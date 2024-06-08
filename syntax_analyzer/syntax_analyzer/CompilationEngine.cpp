//
// Created by Daniel Madjar on 5/29/24.
//
#include "CompilationEngine.h"

std::string CompilationEngine::operators = "+-*/&|<>=";

CompilationEngine::CompilationEngine() {}

std::string CompilationEngine::getToken() {
    return this->currentToken;
}

TokenType CompilationEngine::getType() {
    return this->currentType;
}

void CompilationEngine::getNextToken(JackTokenizer &tokenizer) {
    tokenizer.advance();
    this->currentToken = tokenizer.getToken();
    this->currentType = tokenizer.tokenType(this->currentToken);
}

void CompilationEngine::compileClass(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<class>" << std::endl;
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // 'class'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // className
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '{'

    this->getNextToken(tokenizer);
    while (this->getToken() == "static" || this->getToken() == "field") {
        this->compileClassVarDec(tokenizer, outputFile);
    }

    while (this->getToken() == "constructor" || this->getToken() == "function"
        || this->getToken() == "method") {
        this->compileSubroutine(tokenizer, outputFile);
    }

    tokenizer.printXML(this->getToken(), outputFile); // '}'
    outputFile << "</class>" << std::endl;
}

void CompilationEngine::compileClassVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<classVarDec>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'static' | 'field'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // type
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // varName

    this->getNextToken(tokenizer);
    while (this->getToken() == ",") {
        tokenizer.printXML(this->getToken(), outputFile); // ','
        this->getNextToken(tokenizer);
        tokenizer.printXML(this->getToken(), outputFile); // varName
        this->getNextToken(tokenizer);
    }

    tokenizer.printXML(this->getToken(), outputFile); // ';'

    outputFile << "</classVarDec>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileSubroutine(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<subroutineDec>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'constructor' | 'function' | 'method'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // 'void' | type
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // subroutineName
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '('

    this->getNextToken(tokenizer);
    this->compileParameterList(tokenizer, outputFile); // parameterList
    tokenizer.printXML(this->getToken(), outputFile); // ')'

    this->getNextToken(tokenizer);
    this->compileSubroutineBody(tokenizer, outputFile);

    outputFile << "</subroutineDec>" << std::endl;
}

void CompilationEngine::compileSubroutineBody(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<subroutineBody>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // '{'

    this->getNextToken(tokenizer);
    while (this->getToken() == "var") { // varDec*
       this->compileVarDec(tokenizer, outputFile);
    }

    this->compileStatements(tokenizer, outputFile); // statements

    tokenizer.printXML(this->getToken(), outputFile); // '}'

    outputFile << "</subroutineBody>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<varDec>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'var'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // type
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // varName
    this->getNextToken(tokenizer);
    while (this->getToken() == ",") {
        tokenizer.printXML(this->getToken(), outputFile); // ','
        this->getNextToken(tokenizer);
        tokenizer.printXML(this->getToken(), outputFile); // varName
        this->getNextToken(tokenizer);
    }
    tokenizer.printXML(this->getToken(), outputFile); // ';'
    outputFile << "</varDec>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileStatements(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<statements>" << std::endl;
    std::string token = this->getToken();
    while (this->getToken() == "let" || this->getToken() == "if"
        || this->getToken() == "while" || this->getToken() == "do"
        || this->getToken() == "return") {
        if (this->getToken() == "let") {
            this->compileLet(tokenizer, outputFile);
        } else if (this->getToken() == "if") {
            this->compileIf(tokenizer, outputFile);
        } else if (this->getToken() == "while") {
            this->compileWhile(tokenizer, outputFile);
        } else if (this->getToken() == "do") {
            this->compileDo(tokenizer, outputFile);
        } else if (this->getToken() == "return") {
            this->compileReturn(tokenizer, outputFile);
        }
    }
    outputFile << "</statements>" << std::endl;
}

void CompilationEngine::compileLet(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<letStatement>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'let'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // varName
    this->getNextToken(tokenizer);
    if (this->getToken() == "[") {
        tokenizer.printXML(this->getToken(), outputFile); // '['
        this->getNextToken(tokenizer);
        this->compileExpression(tokenizer, outputFile); // expression
        tokenizer.printXML(this->getToken(), outputFile); // ']'
        this->getNextToken(tokenizer);
    }
    tokenizer.printXML(this->getToken(), outputFile); // '='
    this->getNextToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->getToken(), outputFile); // ';'
    outputFile << "</letStatement>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileDo(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<doStatement>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'do'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // 'subroutineName' | (className | varName)
    this->getNextToken(tokenizer);
    if (this->getToken() == ".") {
        tokenizer.printXML(this->getToken(), outputFile); // '.'
        this->getNextToken(tokenizer);
        tokenizer.printXML(this->getToken(), outputFile); // subroutineName
        this->getNextToken(tokenizer);
    }
    tokenizer.printXML(this->getToken(), outputFile); // '('
    this->compileExpressionList(tokenizer, outputFile);
    tokenizer.printXML(this->getToken(), outputFile); // ')'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // ';'
    outputFile << "</doStatement>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileIf(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<ifStatement>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'if'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '('
    this->getNextToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->getToken(), outputFile); // ')'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '{'
    this->getNextToken(tokenizer);
    this->compileStatements(tokenizer, outputFile); // statements
    tokenizer.printXML(this->getToken(), outputFile); // '}'
    this->getNextToken(tokenizer);
    if (this->getToken() == "else") {
        tokenizer.printXML(this->getToken(), outputFile); // 'else'
        this->getNextToken(tokenizer);
        tokenizer.printXML(this->getToken(), outputFile); // '{'
        this->getNextToken(tokenizer);
        this->compileStatements(tokenizer, outputFile); // statements
        tokenizer.printXML(this->getToken(), outputFile); // '}'
        this->getNextToken(tokenizer);
    }
    outputFile << "</ifStatement>" << std::endl;
}

void CompilationEngine::compileWhile(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<whileStatement>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'while'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '('
    this->getNextToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->getToken(), outputFile); // ')'
    this->getNextToken(tokenizer);
    tokenizer.printXML(this->getToken(), outputFile); // '{'
    this->getNextToken(tokenizer);
    this->compileStatements(tokenizer, outputFile); // statements
    tokenizer.printXML(this->getToken(), outputFile); // '}'
    outputFile << "</whileStatement>" << std::endl;
    this->getNextToken(tokenizer);
}


void CompilationEngine::compileReturn(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<returnStatement>" << std::endl;
    tokenizer.printXML(this->getToken(), outputFile); // 'return'
    this->getNextToken(tokenizer);
    if (this->getToken() != ";") {
        this->compileExpression(tokenizer, outputFile); // expression?
    }
    tokenizer.printXML(this->getToken(), outputFile); // ';'
    outputFile << "</returnStatement>" << std::endl;
    this->getNextToken(tokenizer);
}

void CompilationEngine::compileExpression(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<expression>" << std::endl;
    this->compileTerm(tokenizer, outputFile);
    while (operators.find(this->getToken()) != std::string::npos) {
        tokenizer.printXML(this->getToken(), outputFile);
        this->getNextToken(tokenizer);
        this->compileTerm(tokenizer, outputFile);
    }
    outputFile << "</expression>" << std::endl;
}

void CompilationEngine::compileTerm(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<term>" << std::endl;
    std::string token = this->getToken();
    TokenType type = this->getType();
    if (token == "-" || token == "~") {
        tokenizer.printXML(token, outputFile); // '-' || '~'
        this->getNextToken(tokenizer);
        this->compileTerm(tokenizer, outputFile); /// term
    } else if (token == "true" || token == "false" || token == "null" || token == "this") {
        tokenizer.printXML(token, outputFile); // KeywordConstant
        this->getNextToken(tokenizer);
    } else if (type == TokenType::INT_CONST || type == TokenType::STRING_CONST) {
        tokenizer.printXML(token, outputFile); // integerConstant | stringConstant
        this->getNextToken(tokenizer);
    } else if (token == "(") {
        tokenizer.printXML(token, outputFile); // '('
        this->getNextToken(tokenizer);
        this->compileExpression(tokenizer, outputFile); // expression
        tokenizer.printXML(this->getToken(), outputFile); // ')'
        this->getNextToken(tokenizer);
    } else {
        tokenizer.printXML(token, outputFile); // identifier
        this->getNextToken(tokenizer);
        token = this->getToken();
        if (token == "[") {
            tokenizer.printXML(this->getToken(), outputFile); // '['
            this->getNextToken(tokenizer);
            this->compileExpression(tokenizer, outputFile); // expression
            tokenizer.printXML(this->getToken(), outputFile); // ']'
            this->getNextToken(tokenizer);
        } else if (token == "(") {
            tokenizer.printXML(this->getToken(), outputFile); // '('
            this->compileExpressionList(tokenizer, outputFile); // expressionList
            tokenizer.printXML(this->getToken(), outputFile); // ')'
            this->getNextToken(tokenizer);
        } else if (token == ".") {
            tokenizer.printXML(this->getToken(), outputFile); // '.'
            this->getNextToken(tokenizer);
            tokenizer.printXML(this->getToken(), outputFile); // subroutineName
            this->getNextToken(tokenizer);
            tokenizer.printXML(this->getToken(), outputFile); // '('
            this->compileExpressionList(tokenizer, outputFile); // expressionList
            tokenizer.printXML(this->getToken(), outputFile); // ')'
            this->getNextToken(tokenizer);
        }
    }
    outputFile << "</term>" << std::endl;
}

void CompilationEngine::compileExpressionList(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<expressionList>" << std::endl;
    this->getNextToken(tokenizer);
    if (this->getToken() != ")") {
        this->compileExpression(tokenizer, outputFile);
        while (this->getToken() == ",") {
            tokenizer.printXML(this->getToken(), outputFile); // ','
            this->getNextToken(tokenizer);
            this->compileExpression(tokenizer, outputFile);
        }
    }
    outputFile << "</expressionList>" << std::endl;
}

void CompilationEngine::compileParameterList(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<parameterList>" << std::endl;
    if (this->getToken() != ")") {
        tokenizer.printXML(this->getToken(), outputFile); // type
        this->getNextToken(tokenizer);
        tokenizer.printXML(this->getToken(), outputFile); // varName
        this->getNextToken(tokenizer);
        while (this->getToken() == ",") {
            tokenizer.printXML(this->getToken(), outputFile); // ','
            this->getNextToken(tokenizer);
            tokenizer.printXML(this->getToken(), outputFile); // type
            this->getNextToken(tokenizer);
            tokenizer.printXML(this->getToken(), outputFile); // varName
            this->getNextToken(tokenizer);
        }
    }
    outputFile << "</parameterList>" << std::endl;
}