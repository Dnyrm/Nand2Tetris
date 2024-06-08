//
// Created by Daniel Madjar on 6/2/24.
//

#ifndef TOKENSTATE_H
#define TOKENSTATE_H

#include <iostream>

#include "JackTokenizer.h"

class TokenState {
private:
    std::string currentToken;
    TokenType currentType;
public:
    TokenState();

    std::string getCurrentToken();

    TokenType getCurrentType();

    void setCurrentToken(JackTokenizer &tokenizer);
};

#endif //TOKENSTATE_H
