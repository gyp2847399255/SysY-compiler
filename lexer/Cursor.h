//
// Created by gyp on 2021/9/23.
//

#ifndef SYSYCOMPILER_CURSOR_H
#define SYSYCOMPILER_CURSOR_H
#include "Source.h"
using namespace std;

class Cursor {
    string::iterator now;
    const string::iterator begin;
    const string::iterator end;

public:
    explicit Cursor(Source *content) : begin(content->getBegin()), end(content->getEnd()) {
        now = begin;
    }

    char previous() {
        if (now == begin) {
            return EOF;
        } else {
            return *(now - 1);
        }
    }

    char n_thChar(int n) {
        if (now + n >= end) {
            return EOF;
        } else {
            return *(now + n);
        }
    }

    char first() {
        return n_thChar(0);
    }

    bool isEOF() {
        return now >= end;
    }

    void bump() {
        if (now < end) {
            now++;
        }
    }

    void eatWhile(bool f(char)) {
        while (!isEOF() && f(first())) {
            bump();
        }
    }

    int lengthConsumed() {
        return int(now - begin);
    }
};

#endif //SYSYCOMPILER_CURSOR_H
