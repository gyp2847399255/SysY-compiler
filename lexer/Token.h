//
// Created by gyp on 2021/9/25.
//

#ifndef SYSYCOMPILER_TOKEN_H
#define SYSYCOMPILER_TOKEN_H
#include "SyntaxKind.h"
#include "Source.h"

Source *source;
class Token {
public:
    SyntaxKind kind;
    string content;
    int line;
    Token(SyntaxKind kind, int start, int len) : kind(kind) {
        content = "";
        for (int i = 0; i < len; i++) {
            content += *(source->getBegin() + start + i);
        }
        line = source->getLine(start);
    }

    bool isEOF() const {
        return kind == SyntaxKind::EOF_CHAR;
    }

    bool isError() const {
        return kind == SyntaxKind::ERROR;
    }
};
#endif //SYSYCOMPILER_TOKEN_H
