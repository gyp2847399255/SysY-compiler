//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_VAREXPRNODE_H
#define SYSYCOMPILER_VAREXPRNODE_H
#include "ExprNode.h"
using namespace std;

class VarExprNode : public Node {
    string res;
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::EXPR) {
                ((ExprNode*)i)->print(out, ExprType::EXPR);
            } else {
                i->print(out);
            }
        }
        *out << "<LVal>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        LValSymbol *lValSymbol;
        string *name;
        int cnt = 0;
        ret->isConst = ctx->isGlobal;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
                lValSymbol = symbol.gerVar(name);
                if (lValSymbol == nullptr) {
                    errorList.emplace_back(ErrorKind::UNDEFINED_IDENT, i->getFinishLine());
                } else if (ctx->isLVal && lValSymbol->getConst()) {
                    errorList.emplace_back(ErrorKind::CONSTANT_ASSIGNED, i->getFinishLine());
                }
                ctx->isLVal = false;
            }
            auto ret0 = new ErrorCheckRet;
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::EXPR) {
                cnt++;
            }
            delete ret0;
        }
        if (lValSymbol != nullptr) {
            ret->dimension = lValSymbol->getDimension() - cnt;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        LValSymbol *lValSymbol;
        string *name;
        vector<string*> indices;
        bool lVal = ctx->isLVal;
        ctx->isLVal = false;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
                lValSymbol = symbol.gerVar(name);
            }
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                indices.push_back(ret->res);
            }
        }
        ret->res = &this->res;
        bool flag = true;
        for (auto & i : indices) {
            if (!exprResIsNumber(i)) {
                flag = false;
            }
        }
        if ((lValSymbol->getConst() && flag) || ctx->isGlobal) {
            vector<int> dimension;
            for (auto & i : indices) {
                dimension.push_back(stoi(*i));
            }
            this->res = to_string(lValSymbol->getVal(&dimension));
            return;
        }
        string *offset = nullptr;
        if (!indices.empty()) {
            offset = lValSymbol->calcOffset(&indices);
        } else if (lValSymbol->getGlobal()) {
            offset = new string("0");
        }
        if (lVal) {
            this->res = generateIRVar(name, lValSymbol->getID());
            ret->param = offset;
            ret->isArray = offset != nullptr;
            return;
        }
        if (indices.empty() && !lValSymbol->getGlobal()) {
            this->res = generateIRVar(name, lValSymbol->getID());
        } else {
            res = controlFlowGraphBuilder.tmpVar();
            auto varName = generateIRVar(name, lValSymbol->getID());
            if (indices.size() == lValSymbol->getDimension()) {
                controlFlowGraphBuilder.insert(new LoadWord(&res, &varName, offset));
            } else {
                controlFlowGraphBuilder.insert(new LoadPointer(&res, &varName, offset));
            }
            delete offset;
        }
    }
};
#endif //SYSYCOMPILER_VAREXPRNODE_H
