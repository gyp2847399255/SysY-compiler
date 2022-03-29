//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_EXPRSTMTNODE_H
#define SYSYCOMPILER_EXPRSTMTNODE_H
#include "Node.h"
using namespace std;

class ExprStmtNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        children[0]->buildIR(ctx, ret);
    }
};
#endif //SYSYCOMPILER_EXPRSTMTNODE_H
