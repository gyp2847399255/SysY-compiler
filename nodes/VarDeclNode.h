//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_VARDECLNODE_H
#define SYSYCOMPILER_VARDECLNODE_H
#include "VarDefNode.h"
using namespace std;

class VarDeclNode : public Node {
public:
    void print(ofstream *out) override {
        bool isConst = false;
        if (children[0]->getKind() == SyntaxKind::CONST_KW) {
            isConst = true;
        }
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::VAR_DEF) {
                ((VarDefNode*)i)->print(out, isConst);
            } else {
                i->print(out);
            }
        }
        if (isConst) {
            *out << "<ConstDecl>" << endl;
        } else {
            *out << "<VarDecl>" << endl;
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        ctx->isConst = children[0]->getKind() == SyntaxKind::CONST_KW;
        for (auto & i : children) {
            i->checkError(ctx, ret);
        }
        ctx->isConst = false;
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        for (auto & i : children) {
            ctx->isConst = children[0]->getKind() == SyntaxKind::CONST_KW;
            i->buildIR(ctx, ret);
        }
        ctx->isConst = false;
    }
};
#endif //SYSYCOMPILER_VARDECLNODE_H
