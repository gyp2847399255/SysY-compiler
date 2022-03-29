//
// Created by gyp on 2021/9/25.
//

#ifndef SYSYCOMPILER_LEXERERROR_H
#define SYSYCOMPILER_LEXERERROR_H
#include "Token.h"

class LexerError {
    int lineNum;
public:
    explicit LexerError(Token* token) {
        lineNum = token->line;
    }
    void printError() const {
        printf("Lexer Error on line %d\n", lineNum);
    }
};
#endif //SYSYCOMPILER_LEXERERROR_H
