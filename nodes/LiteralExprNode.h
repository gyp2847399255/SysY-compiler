//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_LITERALEXPRNODE_H
#define SYSYCOMPILER_LITERALEXPRNODE_H
#include "Node.h"
using namespace std;

class LiteralExprNode : public Node {
    string res;
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
        *out << "<Number>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        string *content;
        if (children[0]->getKind() == SyntaxKind::INTEGER) {
            content = ((TokenNode*)children[0])->getContent();
            for (auto i : *content) {
                ret->val = ret->val * 10 + i - '0';
            }
            ret->isConst = true;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        this->res = *((TokenNode*)children[0])->getContent();
        ret->res = &this->res;
    }
};
#endif //SYSYCOMPILER_LITERALEXPRNODE_H
