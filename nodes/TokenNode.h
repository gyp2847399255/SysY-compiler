//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_TOKENNODE_H
#define SYSYCOMPILER_TOKENNODE_H
#include "Node.h"
using namespace std;

class TokenNode : public Node {
    string *content;
public:
    TokenNode(SyntaxKind kind, string* content, int line) : content(content) {
        this->setNodeElement(kind, line);
    }

    void print(ofstream *out) override {
        *out << syntaxKind2Name[kind] << " " << *content << endl;
    }

    string *getContent() {
        return content;
    }
};
#endif //SYSYCOMPILER_TOKENNODE_H
