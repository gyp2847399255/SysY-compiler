//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_STATEMENTNODE_H
#define SYSYCOMPILER_STATEMENTNODE_H
#include "Node.h"
using namespace std;

class StatementNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
        *out << "<Stmt>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            i->checkError(ctx, ret);
            ret->isReturn = i->getKind() == SyntaxKind::RETURN_STMT;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        BasicBlock *continueCFG = ctx->continueBlock;
        BasicBlock *curBB = controlFlowGraphBuilder.getCur();
        bool stmtAfterLoop = ctx->stmtAfterLoop;
        if (stmtAfterLoop) {
            ctx->continueBlock = controlFlowGraphBuilder.newBasicBlock();
        }
        bool needJumpElse = ctx->needJumpElse;
        ctx->stmtAfterLoop = false;
        ctx->needJumpElse = false;
        for (auto & i : children) {
            i->buildIR(ctx, ret);
        }
        if (stmtAfterLoop) {
            controlFlowGraphBuilder.changeCur(ctx->continueBlock);
            ctx->condition->buildIR(ctx, ret);
            controlFlowGraphBuilder.insert(new JumpWhenTrue(curBB, ret->res));
        } else if (needJumpElse) {
            controlFlowGraphBuilder.insert(new Jump(ctx->elseFinalBlock));
        }
        ctx->continueBlock = continueCFG;
    }
};
#endif //SYSYCOMPILER_STATEMENTNODE_H
