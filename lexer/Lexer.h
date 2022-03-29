//
// Created by gyp on 2021/9/23.
//

#ifndef SYSYCOMPILER_LEXER_H
#define SYSYCOMPILER_LEXER_H
#include "Source.h"
#include "Cursor.h"
#include "Token.h"
#include "LexerError.h"

class Lexer {
    Cursor *cursor;
    vector<LexerError*> errors;
private:
    SyntaxKind tokenKind() {
        char first = cursor->first();
        if (first == EOF) {
            return SyntaxKind::EOF_CHAR;
        }
        cursor->bump();
        switch (first) {
            case '/' :
                if (cursor->first() == '/') {
                    eatLineComment();
                    return SyntaxKind::LINE_COMMENT;
                } else if (cursor->first() == '*') {
                    eatBlockComment();
                    return SyntaxKind::BLOCK_COMMENT;
                }
                return SyntaxKind::DIV;
            case ',' :
                return SyntaxKind::COMMA;
            case ';':
                return SyntaxKind::SEMICOLON;
            case '+':
                return SyntaxKind::PLUS;
            case '-':
                return SyntaxKind::MINUS;
            case '*':
                return SyntaxKind::MULTI;
            case '%':
                return SyntaxKind::MOD;
            case '{':
                return SyntaxKind::L_CURLY;
            case '}':
                return SyntaxKind::R_CURLY;
            case '[':
                return SyntaxKind::L_BRACKET;
            case ']':
                return SyntaxKind::R_BRACKET;
            case '(':
                return SyntaxKind::L_PAREN;
            case ')':
                return SyntaxKind::R_PAREN;
            case '!':
                if (cursor->first() == '=') {
                    cursor->bump();
                    return SyntaxKind::NEQ;
                } else {
                    return SyntaxKind::NOT_LOGIC;
                }
            case '=':
                if (cursor->first() == '=') {
                    cursor->bump();
                    return SyntaxKind::EQ;
                } else {
                    return SyntaxKind::ASSIGN;
                }
            case '<':
                if (cursor->first() == '=') {
                    cursor->bump();
                    return SyntaxKind::LE;
                } else {
                    return SyntaxKind::LT;
                }
            case '>':
                if (cursor->first() == '=') {
                    cursor->bump();
                    return SyntaxKind::GE;
                } else {
                    return SyntaxKind::GT;
                }
            case '&':
                if (cursor->first() == '&') {
                    cursor->bump();
                    return SyntaxKind::AND_LOGIC;
                }
                return SyntaxKind::ERROR;
            case '|':
                if (cursor->first() == '|') {
                    cursor->bump();
                    return SyntaxKind::OR_LOGIC;
                }
                return SyntaxKind::ERROR;
            case '"':
                eatString();
                return SyntaxKind::STRING;
            default:
                if (isdigit(first)) {
                    cursor->eatWhile([] (char c) {
                        return bool (isdigit(c));
                    });
                    return SyntaxKind::INTEGER;
                }
                if (isIdentStart(first)) {
                    string ident;
                    ident += first;
                    while (isIdentContinue(cursor->first())) {
                        ident += cursor->first();
                        cursor->bump();
                    }
                    return judgeIdent(&ident);
                } else {
                    return SyntaxKind::ERROR;
                }
        }
    }

    void eatLineComment() {
        cursor -> eatWhile([] (char c) {
            return c != '\n';
        });
        cursor -> bump();
    }

    void eatBlockComment() {
        cursor->bump();
        cursor->bump();
        cursor -> eatWhile([] (char c) {
            return c != '/';
        });
        while (cursor->previous() != '*' && cursor->first() != EOF) {
            cursor->bump();
            cursor ->eatWhile([] (char c) {
                return c != '/';
            });
        }
        cursor -> bump();
    }

    void eatString() {
        bool afterBackslash = false;
        while (true) {
            char c = cursor->first();
            switch (c) {
                case EOF :
                    return;
                case '"' :
                    if (afterBackslash) {
                        cursor -> bump();
                        afterBackslash = false;
                    } else {
                        cursor -> bump();
                        return;
                    }
                    break;
                case '\\' :
                    if (afterBackslash) {
                        cursor -> bump();
                        afterBackslash = false;
                    } else {
                        cursor -> bump();
                        afterBackslash = true;
                    }
                    break;
                default :
                    afterBackslash = false;
                    cursor -> bump();
            }
        }
    }

    static bool isIdentStart(char c) {
        return isalpha(c) || c == '_';
    }

    static bool isIdentContinue(char c) {
        return isIdentStart(c) || isdigit(c);
    }

    Token* nextToken() {
        cursor->eatWhile([] (char c) {
            return bool(isspace(c));
        });
        int start = cursor->lengthConsumed();
        SyntaxKind kind = tokenKind();
        while (kind == SyntaxKind::LINE_COMMENT || kind == SyntaxKind::BLOCK_COMMENT) {
            cursor->eatWhile([] (char c) {
                return bool(isspace(c));
            });
            start = cursor->lengthConsumed();
            kind = tokenKind();
        }
        int length = cursor->lengthConsumed() - start;
        return new Token(kind, start, length);
    }

public:
    explicit Lexer(Source *content) {
        cursor = new Cursor(content);
    }

    vector<Token*> tokenize() {
        Token *token = nextToken();
        vector<Token*> res;
        res.clear();
        while (!token->isEOF()) {
            if (token->isError()) {
                errors.push_back(new LexerError(token));
            } else {
                res.push_back(token);
            }
            token = nextToken();
        }
        res.push_back(token);
        return res;
    }

    bool hasError() {
        return !errors.empty();
    }

    void printError() {
        cout << "Lexer Error" << endl;
        for (auto & error : errors) {
            error->printError();
        }
    }
};

#endif //SYSYCOMPILER_LEXER_H
