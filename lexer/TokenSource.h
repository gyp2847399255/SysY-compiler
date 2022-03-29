//
// Created by gyp on 2021/9/28.
//

#ifndef SYSYCOMPILER_TOKENSOURCE_H
#define SYSYCOMPILER_TOKENSOURCE_H
#include "Token.h"
class TokenSource {
    vector<Token*> tokens;
    vector<Token*>::iterator cur;

public:
    explicit TokenSource(vector<Token*> *tokens) {
        this->tokens = *tokens;
        cur = this->tokens.begin();
    }

    Token* first() {
        return nthToken(0);
    }

    Token* nthToken(int n) {
        if (cur + n >= tokens.end()) {
            return nullptr;
        } else {
            return *(cur + n);
        }
    }

    int lastLine() {
        if (cur == tokens.begin()) {
            return 0;
        }
        return nthToken(-1)->line;
    }

    void bump() {
        if (cur != tokens.end()) {
            cur++;
        }
    }
};
#endif //SYSYCOMPILER_TOKENSOURCE_H
