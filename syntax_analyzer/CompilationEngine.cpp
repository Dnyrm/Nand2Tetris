//
// Created by Daniel Madjar on 5/29/24.
//
#include "CompilationEngine.h"

std::string CompilationEngine::operators = "+-*/&|<>=";

CompilationEngine::CompilationEngine() {}

void CompilationEngine::compileClass(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<class>" << std::endl;
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'class'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // className
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '{'

    this->state.setCurrentToken(tokenizer);
    while (this->state.getCurrentToken() == "static" || this->state.getCurrentToken() == "field") {
        this->compileClassVarDec(tokenizer, outputFile);
    }

    while (this->state.getCurrentToken() == "constructor" || this->state.getCurrentToken() == "function"
        || this->state.getCurrentToken() == "method") {
        this->compileSubroutine(tokenizer, outputFile);
    }

    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '}'
    outputFile << "</class>" << std::endl;
}

void CompilationEngine::compileClassVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<classVarDec>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'static' | 'field'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // type
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName

    this->state.setCurrentToken(tokenizer);
    while (this->state.getCurrentToken() == ",") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ','
        this->state.setCurrentToken(tokenizer);
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
        this->state.setCurrentToken(tokenizer);
    }

    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ';'

    outputFile << "</classVarDec>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileSubroutine(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<subroutineDec>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'constructor' | 'function' | 'method'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'void' | type
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // subroutineName
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('

    this->state.setCurrentToken(tokenizer);
    this->compileParameterList(tokenizer, outputFile); // parameterList
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'

    this->state.setCurrentToken(tokenizer);
    this->compileSubroutineBody(tokenizer, outputFile);

    outputFile << "</subroutineDec>" << std::endl;
}

void CompilationEngine::compileSubroutineBody(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<subroutineBody>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '{'

    this->state.setCurrentToken(tokenizer);
    while (this->state.getCurrentToken() == "var") { // varDec*
       this->compileVarDec(tokenizer, outputFile);
    }

    this->compileStatements(tokenizer, outputFile); // statements

    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '}'

    outputFile << "</subroutineBody>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileVarDec(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<varDec>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'var'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // type
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
    this->state.setCurrentToken(tokenizer);
    while (this->state.getCurrentToken() == ",") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ','
        this->state.setCurrentToken(tokenizer);
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
        this->state.setCurrentToken(tokenizer);
    }
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ';'
    outputFile << "</varDec>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileStatements(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<statements>" << std::endl;
    std::string token = this->state.getCurrentToken();
    while (this->state.getCurrentToken() == "let" || this->state.getCurrentToken() == "if"
        || this->state.getCurrentToken() == "while" || this->state.getCurrentToken() == "do"
        || this->state.getCurrentToken() == "return") {
        if (this->state.getCurrentToken() == "let") {
            this->compileLet(tokenizer, outputFile);
        } else if (this->state.getCurrentToken() == "if") {
            this->compileIf(tokenizer, outputFile);
        } else if (this->state.getCurrentToken() == "while") {
            this->compileWhile(tokenizer, outputFile);
        } else if (this->state.getCurrentToken() == "do") {
            this->compileDo(tokenizer, outputFile);
        } else if (this->state.getCurrentToken() == "return") {
            this->compileReturn(tokenizer, outputFile);
        }
    }
    outputFile << "</statements>" << std::endl;
}

void CompilationEngine::compileLet(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<letStatement>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'let'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
    this->state.setCurrentToken(tokenizer);
    if (this->state.getCurrentToken() == "[") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '['
        this->state.setCurrentToken(tokenizer);
        this->compileExpression(tokenizer, outputFile); // expression
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ']'
        this->state.setCurrentToken(tokenizer);
    }
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '='
    this->state.setCurrentToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ';'
    outputFile << "</letStatement>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileDo(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<doStatement>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'do'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'subroutineName' | (className | varName)
    this->state.setCurrentToken(tokenizer);
    if (this->state.getCurrentToken() == ".") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '.'
        this->state.setCurrentToken(tokenizer);
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // subroutineName
        this->state.setCurrentToken(tokenizer);
    }
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('
    this->compileExpressionList(tokenizer, outputFile);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ';'
    outputFile << "</doStatement>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileIf(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<ifStatement>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'if'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('
    this->state.setCurrentToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '{'
    this->state.setCurrentToken(tokenizer);
    this->compileStatements(tokenizer, outputFile); // statements
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '}'
    this->state.setCurrentToken(tokenizer);
    if (this->state.getCurrentToken() == "else") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'else'
        this->state.setCurrentToken(tokenizer);
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '{'
        this->state.setCurrentToken(tokenizer);
        this->compileStatements(tokenizer, outputFile); // statements
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '}'
        this->state.setCurrentToken(tokenizer);
    }
    outputFile << "</ifStatement>" << std::endl;
}

void CompilationEngine::compileWhile(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<whileStatement>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'while'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('
    this->state.setCurrentToken(tokenizer);
    this->compileExpression(tokenizer, outputFile); // expression
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
    this->state.setCurrentToken(tokenizer);
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '{'
    this->state.setCurrentToken(tokenizer);
    this->compileStatements(tokenizer, outputFile); // statements
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '}'
    outputFile << "</whileStatement>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}


void CompilationEngine::compileReturn(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<returnStatement>" << std::endl;
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // 'return'
    this->state.setCurrentToken(tokenizer);
    if (this->state.getCurrentToken() != ";") {
        this->compileExpression(tokenizer, outputFile); // expression?
    }
    tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ';'
    outputFile << "</returnStatement>" << std::endl;
    this->state.setCurrentToken(tokenizer);
}

void CompilationEngine::compileExpression(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<expression>" << std::endl;
    this->compileTerm(tokenizer, outputFile);
    while (operators.find(this->state.getCurrentToken()) != std::string::npos) {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile);
        this->state.setCurrentToken(tokenizer);
        this->compileTerm(tokenizer, outputFile);
    }
    outputFile << "</expression>" << std::endl;
}

void CompilationEngine::compileTerm(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<term>" << std::endl;
    std::string token = this->state.getCurrentToken();
    TokenType type = this->state.getCurrentType();
    if (token == "-" || token == "~") {
        tokenizer.printXML(token, outputFile); // '-' || '~'
        this->state.setCurrentToken(tokenizer);
        this->compileTerm(tokenizer, outputFile); /// term
    } else if (token == "true" || token == "false" || token == "null" || token == "this") {
        tokenizer.printXML(token, outputFile); // KeywordConstant
        this->state.setCurrentToken(tokenizer);
    } else if (type == TokenType::INT_CONST || type == TokenType::STRING_CONST) {
        tokenizer.printXML(token, outputFile); // integerConstant | stringConstant
        this->state.setCurrentToken(tokenizer);
    } else if (token == "(") {
        tokenizer.printXML(token, outputFile); // '('
        this->state.setCurrentToken(tokenizer);
        this->compileExpression(tokenizer, outputFile); // expression
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
        this->state.setCurrentToken(tokenizer);
    } else {
        tokenizer.printXML(token, outputFile); // identifier
        this->state.setCurrentToken(tokenizer);
        token = this->state.getCurrentToken();
        if (token == "[") {
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '['
            this->state.setCurrentToken(tokenizer);
            this->compileExpression(tokenizer, outputFile); // expression
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ']'
            this->state.setCurrentToken(tokenizer);
        } else if (token == "(") {
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('
            this->compileExpressionList(tokenizer, outputFile); // expressionList
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
            this->state.setCurrentToken(tokenizer);
        } else if (token == ".") {
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '.'
            this->state.setCurrentToken(tokenizer);
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // subroutineName
            this->state.setCurrentToken(tokenizer);
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // '('
            this->compileExpressionList(tokenizer, outputFile); // expressionList
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ')'
            this->state.setCurrentToken(tokenizer);
        }
    }
    outputFile << "</term>" << std::endl;
}

void CompilationEngine::compileExpressionList(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<expressionList>" << std::endl;
    this->state.setCurrentToken(tokenizer);
    if (this->state.getCurrentToken() != ")") {
        this->compileExpression(tokenizer, outputFile);
        while (this->state.getCurrentToken() == ",") {
            tokenizer.printXML(state.getCurrentToken(), outputFile); // ','
            this->state.setCurrentToken(tokenizer);
            this->compileExpression(tokenizer, outputFile);
        }
    }
    outputFile << "</expressionList>" << std::endl;
}

void CompilationEngine::compileParameterList(JackTokenizer &tokenizer, std::ofstream &outputFile) {
    outputFile << "<parameterList>" << std::endl;
    if (this->state.getCurrentToken() != ")") {
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // type
        this->state.setCurrentToken(tokenizer);
        tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
        this->state.setCurrentToken(tokenizer);
        while (this->state.getCurrentToken() == ",") {
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // ','
            this->state.setCurrentToken(tokenizer);
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // type
            this->state.setCurrentToken(tokenizer);
            tokenizer.printXML(this->state.getCurrentToken(), outputFile); // varName
            this->state.setCurrentToken(tokenizer);
        }
    }
    outputFile << "</parameterList>" << std::endl;
}