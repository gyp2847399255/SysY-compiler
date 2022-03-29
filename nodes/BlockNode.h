//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_BLOCKNODE_H
#define SYSYCOMPILER_BLOCKNODE_H
#include "Node.h"
using namespace std;

class BlockNode : public Node {
public:
    void print(ofstream *out) override {
        children[0]->print(out);
        for (int i = 1; i < children.size() - 1; i++) {
            children[i]->print(out);
        }
        children.back()->print(out);
        *out << "<Block>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        if (ctx->afterFuncDef) {
            ctx->afterFuncDef = false;
        } else {
            symbol.startBlock();
        }
        bool flag = false;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::STMT) {
                flag = true;
            }
            i->checkError(ctx, ret);
        }
        if (!flag) {
            ret->isReturn = false;
        }
        symbol.finishBlock();
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        if (ctx->afterFuncDef) {
            ctx->afterFuncDef = false;
        } else {
            symbol.startBlock();
        }
        for (auto & i : children) {
            i->buildIR(ctx, ret);
        }
        symbol.finishBlock();
    }
};
#endif //SYSYCOMPILER_BLOCKNODE_H
