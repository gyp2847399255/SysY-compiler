//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_INITVALNODE_H
#define SYSYCOMPILER_INITVALNODE_H
#include "ExprNode.h"
using namespace std;

class InitValNode : public Node {
public:
    void print(ofstream *out, bool isConst) {
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::EXPR) {
                ((ExprNode*)i)->print(out, isConst ? ExprType::CONST_EXPR : ExprType::EXPR);
            } else if (i->getKind() == SyntaxKind::INIT_VAL) {
                ((InitValNode*)i)->print(out, isConst);
            } else {
                i->print(out);
            }
        }
        if (isConst) {
            *out << "<ConstInitVal>" << endl;
        } else {
            *out << "<InitVal>" << endl;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        vector<string*> init;
        for (auto & i : children) {
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                init.push_back(ret->res);
            } else if (i->getKind() == SyntaxKind::INIT_VAL) {
                for (auto & j : ret->init) {
                    init.push_back(j);
                }
            }
        }
        ret->init = init;
    }
};
#endif //SYSYCOMPILER_INITVALNODE_H
