//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_ASSIGNSTMTNODE_H
#define SYSYCOMPILER_ASSIGNSTMTNODE_H
#include "ExprNode.h"
using namespace std;

class AssignStmtNode : public Node {
public:
    void print(ofstream *out) override {
        if (children[0]->getKind() == SyntaxKind::EXPR) {
            ((ExprNode*)children[0])->print(out, ExprType::L_VAL);
        }
        for (int i = 1; i < children.size(); i++) {
            children[i]->print(out);
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        if (children[0]->getKind() == SyntaxKind::EXPR) {
            ctx->isLVal = true;
            children[0]->checkError(ctx, ret);
            ctx->isLVal = false;
        }
        for (int i = 1; i < children.size(); i++) {
            children[i]->checkError(ctx, ret);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        ctx->isLVal = true;
        children[0]->buildIR(ctx, ret);
        bool isArray = ret->isArray;
        string *lVal = ret->res;
        string *offset = ret->param;
        ret->param = nullptr;
        string *integer;
        bool getChar = false;
        for (int i = 1; i < children.size(); i++) {
            children[i]->buildIR(ctx, ret);
            if (children[i]->getKind() == SyntaxKind::EXPR) {
                integer = ret->res;
            } else if (children[i]->getKind() == SyntaxKind::GETCHAR_KW) {
                integer = new string(controlFlowGraphBuilder.tmpVar());
                controlFlowGraphBuilder.insert(new ReadChar());
                controlFlowGraphBuilder.insert(new Getchar(integer));
                getChar = true;
            }
        }
        if (isArray) {
            controlFlowGraphBuilder.insert(new StoreWord(integer, lVal, offset));
            delete offset;
        } else {
            controlFlowGraphBuilder.insert(new Assign(lVal, integer));
        }
        if (getChar) {
            delete integer;
        }
    }
};
#endif //SYSYCOMPILER_ASSIGNSTMTNODE_H
