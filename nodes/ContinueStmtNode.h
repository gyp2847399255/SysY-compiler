//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_CONTINUESTMTNODE_H
#define SYSYCOMPILER_CONTINUESTMTNODE_H
#include "Node.h"
#include "TokenNode.h"
using namespace std;

class ContinueStmtNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            i->checkError(ctx, ret);
        }
        if (ctx->loopNum == 0) {
            errorList.emplace_back(ErrorKind::BREAK_CONTINUE_OUT_LOOP, children[0]->getStartLine());
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        controlFlowGraphBuilder.insert(new Jump(ctx->continueBlock));
    }
};
#endif //SYSYCOMPILER_CONTINUESTMTNODE_H
