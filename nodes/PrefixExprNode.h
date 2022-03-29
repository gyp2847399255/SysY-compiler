//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_PREFIXEXPRNODE_H
#define SYSYCOMPILER_PREFIXEXPRNODE_H
#include "PrefixOPNode.h"
using namespace std;

enum class PrefixExprType {
    L_VAL,
    PREFIX_EXPR
};

class PrefixExprNode : public Node {
    string res;
    static set<SyntaxKind> expr;
public:
    void print(ofstream *out, PrefixExprType type) {
        if (type == PrefixExprType::L_VAL) {
            children[0]->print(out);
            return;
        }
        int size = 1;
        if (children[0]->getKind() == SyntaxKind::PREFIX_OP) {
            size = ((PrefixOPNode*)children[0])->getSize() + 1;
        }
        bool flag = false;
        for (auto & i : children) {
            i->print(out);
            if (i->getKind() == SyntaxKind::FUNC_EXPR) {
                flag = true;
            }
        }
        if (!flag) {
            *out << "<PrimaryExp>" << endl;
        }
        for (int i = 0; i < size; i++) {
            *out << "<UnaryExp>" << endl;
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            i->checkError(ctx, ret);
            if (i->getKind() != SyntaxKind::PREFIX_OP) {
                i->checkError(ctx, ret);
            }
        }
        if (children[0]->getKind() == SyntaxKind::PREFIX_OP) {
            children[0]->checkError(ctx, ret);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        if (ctx->isLVal) {
            children[0]->buildIR(ctx, ret);
            this->res = *ret->res;
            return;
        }
        for (auto & i : children) {
            if (expr.count(i->getKind())) {
                i->buildIR(ctx, ret);
                ctx->res = ret->res;
            }
        }
        ctx->isConst = exprResIsNumber(ret->res);
        if (ctx->isConst || children[0]->getKind() != SyntaxKind::PREFIX_OP) {
            res = *ret->res;
        } else {
            res = controlFlowGraphBuilder.tmpVar();
            controlFlowGraphBuilder.insert(new Assign(&res, ret->res));
        }
        if (children[0]->getKind() == SyntaxKind::PREFIX_OP) {
            ctx->res = &res;
            children[0]->buildIR(ctx, ret);
        }
        ctx->isConst = false;
        ret->res = &res;
    }
};

set<SyntaxKind> PrefixExprNode::expr = {
        SyntaxKind::EXPR, SyntaxKind::VAR_EXPR, SyntaxKind::LITERAL_EXPR, SyntaxKind::FUNC_EXPR
};
#endif //SYSYCOMPILER_PREFIXEXPRNODE_H