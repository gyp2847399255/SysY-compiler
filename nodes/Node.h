//
// Created by gyp on 2021/9/28.
//

#ifndef SYSYCOMPILER_NODE_H
#define SYSYCOMPILER_NODE_H
#include "../lexer/lib.h"
#include "../parser/ErrorKind.h"
#include "../parser/Symbol.h"
#include "../parser/Context.h"
#include "../control-flow-graphs/lib.h"
using namespace std;

class Node {
protected:
    int startLine;
    int finishLine;
    SyntaxKind kind;
    vector<Node*> children;
public:
    void setNodeElement(SyntaxKind syntaxKind, int line) {
        this->kind = syntaxKind;
        this->startLine = this->finishLine = line;
    }

    void setNodeElement(SyntaxKind syntaxKind) {
        this->kind = syntaxKind;
        this->startLine = children[0]->startLine;
        this->finishLine = children[children.size() - 1]->finishLine;
    }

    int getStartLine() const {
        return startLine;
    }

    int getFinishLine() const {
        return finishLine;
    }

    void addChild(Node* child) {
        children.insert(children.begin(), child);
    }

    SyntaxKind getKind() {
        return this->kind;
    }

    virtual void print(ofstream* out) {
        *out << syntaxKind2Name[kind] << " Use Wrong print" << endl;
    }

    virtual void checkError(ErrorCheckCtx* ctx, ErrorCheckRet *ret) {
        for (auto & i : children) {
            i->checkError(ctx, ret);
        }
    }

    virtual void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) {}
};

#endif //SYSYCOMPILER_NODE_H