//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_WHILESTMTNODE_H
#define SYSYCOMPILER_WHILESTMTNODE_H
#include "ExprNode.h"
using namespace std;

class WhileStmtNode : public Node {
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

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        ctx->loopNum++;
        for (auto & i : children) {
            i->checkError(ctx, ret);
        }
        ctx->loopNum--;
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        BasicBlock *cfg2 = ctx->breakBlock;
        bool inLoop = ctx->stmtAfterLoop;
        children[2]->buildIR(ctx, ret);
        BasicBlock *final = controlFlowGraphBuilder.newBasicBlock();
        ExprNode* condition = ctx->condition;
        controlFlowGraphBuilder.insert(new JumpWhenFalse(final, ret->res));
        BasicBlock *body = controlFlowGraphBuilder.newBasicBlock();
        controlFlowGraphBuilder.changeCur(body);
        ctx->breakBlock = final;
        ctx->stmtAfterLoop = true;
        ctx->condition = (ExprNode*) children[2];
        children[4]->buildIR(ctx, ret);
        ctx->breakBlock = cfg2;
        ctx->stmtAfterLoop = inLoop;
        ctx->condition = condition;
        controlFlowGraphBuilder.changeCur(final);
    }
};
#endif //SYSYCOMPILER_WHILESTMTNODE_H