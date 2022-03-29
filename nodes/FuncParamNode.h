//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCPARAMNODE_H
#define SYSYCOMPILER_FUNCPARAMNODE_H
#include "ExprNode.h"
using namespace std;

class FuncParamNode : public Node {
public:
    void print(ofstream *out) override {
        children[0]->print(out);
        for (int i = 1; i < children.size(); i++) {
            if (children[i]->getKind() == SyntaxKind::EXPR) {
                ((ExprNode*)children[i])->print(out, ExprType::CONST_EXPR);
            } else {
                children[i]->print(out);
            }
        }
        *out << "<FuncFParam>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        string *name;
        int line;
        vector<int> dimension;
        bool flag = false;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
                line = i->getFinishLine();
            }
            auto ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (flag) {
                flag = false;
                dimension.push_back(ret0->val);
            }
            delete ret0;
            if (i->getKind() == SyntaxKind::L_BRACKET) {
                flag = true;
            }
        }
        ret->dimension = dimension.size();
        if (!symbol.addVar(false, &dimension, name)) {
            errorList.emplace_back(ErrorKind::REDEFINE_IDENT, line);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        string *name;
        vector<int> dimension;
        ret->clear();
        int val = 0;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
            }
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::R_BRACKET) {
                dimension.push_back(val);
            } else if (i->getKind() == SyntaxKind::EXPR) {
                if (exprResIsNumber(ret->res)) {
                    val = stoi(*ret->res);
                } else {
                    val = 0;
                }
            }
        }
        int id = symbol.addVar(false, &dimension, name);
        ret->param = new string(generateIRVar(name, id));
    }
};
#endif //SYSYCOMPILER_FUNCPARAMNODE_H
