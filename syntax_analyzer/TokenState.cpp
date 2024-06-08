#include "TokenState.h"
//
// Created by Daniel Madjar on 6/2/24.
//
TokenState::TokenState() {}

std::string TokenState::getCurrentToken() {
    return this->currentToken;
}

TokenType TokenState::getCurrentType() {
    return this->currentType;
}

void TokenState::setCurrentToken(JackTokenizer &tokenizer) {
    tokenizer.advance();
    this->currentToken = tokenizer.getToken();
    this->currentType = tokenizer.tokenType(this->currentToken);
}