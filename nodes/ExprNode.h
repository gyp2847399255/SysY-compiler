//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_EXPRNODE_H
#define SYSYCOMPILER_EXPRNODE_H
#include "BinExprNode.h"
using namespace std;

enum class ExprType {
    CONST_EXPR,
    CONDITION,
    EXPR,
    L_VAL
};

class ExprNode : public Node {
    string res;
public:
    void print(ofstream *out, ExprType type) {
        switch (type) {
            case ExprType::EXPR:
            case ExprType::CONST_EXPR:
                if (children[0]->getKind() == SyntaxKind::BIN_EXPR) {
                    ((BinExprNode*)children[0])->print(out, BinExprType::ADD_EXPR);
                } else if (children[0]->getKind() == SyntaxKind::PREFIX_EXPR) {
                    ((PrefixExprNode*)children[0])->print(out, PrefixExprType::PREFIX_EXPR);
                    *out << "<MulExp>" << endl;
                    *out << "<AddExp>" << endl;
                }
                break;
            case ExprType::L_VAL:
                if (children[0]->getKind() == SyntaxKind::PREFIX_EXPR) {
                    ((PrefixExprNode*)children[0])->print(out, PrefixExprType::L_VAL);
                }
                break;
            case ExprType::CONDITION:
                if (children[0]->getKind() == SyntaxKind::BIN_EXPR) {
                    ((BinExprNode*)children[0])->print(out, BinExprType::OR_EXPR);
                } else if (children[0]->getKind() == SyntaxKind::PREFIX_EXPR) {
                    ((PrefixExprNode*)children[0])->print(out, PrefixExprType::PREFIX_EXPR);
                    *out << "<MulExp>" << endl;
                    *out << "<AddExp>" << endl;
                    *out << "<RelExp>" << endl;
                    *out << "<EqExp>" << endl;
                    *out << "<LAndExp>" << endl;
                    *out << "<LOrExp>" << endl;
                }
                break;
            default:
                break;
        }
        switch (type) {
            case ExprType::EXPR:
                *out << "<Exp>" << endl;
                break;
            case ExprType::CONDITION:
                *out << "<Cond>" << endl;
                break;
            case ExprType::CONST_EXPR:
                *out << "<ConstExp>" << endl;
                break;
            default:
                break;
        }
    }

    void print(ofstream *out) override {
        print(out, ExprType::EXPR);
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            auto ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (ret0->isConst) {
                ret->isConst = true;
            }
            ret->dimension = ret0->dimension;
            delete ret0;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        children[0]->buildIR(ctx, ret);
        this->res = *ret->res;
        ret->res = &this->res;
    }
};
#endif //SYSYCOMPILER_EXPRNODE_H
