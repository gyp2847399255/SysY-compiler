//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_IFSTMTNODE_H
#define SYSYCOMPILER_IFSTMTNODE_H
#include "ExprNode.h"
using namespace std;

class IfStmtNode : public Node {
public:
    void print(ofstream *out) override {
        for (int i = 0; i < children.size(); i++) {
            if (i == 2 && children[i]->getKind() == SyntaxKind::EXPR) {
                ((ExprNode*)children[i])->print(out, ExprType::CONDITION);
            } else {
                children[i]->print(out);
            }
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        BasicBlock *cfg3 = ctx->elseFinalBlock;
        bool needJumpElse = ctx->needJumpElse;
        children[2]->buildIR(ctx, ret);
        bool hasElse = false;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::ELSE_KW) {
                hasElse = true;
                break;
            }
        }
        if (hasElse) {
            BasicBlock *elseCFG = controlFlowGraphBuilder.newBasicBlock();
            controlFlowGraphBuilder.insert(new JumpWhenFalse(elseCFG, ret->res));
            controlFlowGraphBuilder.changeCur(controlFlowGraphBuilder.newBasicBlock());
            ctx->needJumpElse = true;
            BasicBlock *finalCFG = controlFlowGraphBuilder.newBasicBlock();
            ctx->elseFinalBlock = finalCFG;
            children[4]->buildIR(ctx, ret);
            controlFlowGraphBuilder.changeCur(elseCFG);
            ctx->needJumpElse = false;
            children[6]->buildIR(ctx, ret);
            controlFlowGraphBuilder.changeCur(finalCFG);
        } else {
            BasicBlock *finalCFG = controlFlowGraphBuilder.newBasicBlock();
            controlFlowGraphBuilder.insert(new JumpWhenFalse(finalCFG, ret->res));
            controlFlowGraphBuilder.changeCur(controlFlowGraphBuilder.newBasicBlock());
            ctx->needJumpElse = false;
            children[4]->buildIR(ctx, ret);
            controlFlowGraphBuilder.changeCur(finalCFG);
        }
        ctx->elseFinalBlock = cfg3;
        ctx->needJumpElse = needJumpElse;
    }
};
#endif //SYSYCOMPILER_IFSTMTNODE_H
