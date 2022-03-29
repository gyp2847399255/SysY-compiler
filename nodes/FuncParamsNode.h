//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCPARAMSNODE_H
#define SYSYCOMPILER_FUNCPARAMSNODE_H
#include "Node.h"
using namespace std;

class FuncParamsNode : public Node {
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
        *out << "<FuncFParams>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            auto ret0 = new ErrorCheckRet;
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::FUNC_PARAM) {
                ret->args.push_back(ret0->dimension);
            }
            delete ret0;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        ret->clear();
        vector<string*> args;
        for (auto & i : children) {
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::FUNC_PARAM) {
                args.push_back(ret->param);
            }
        }
        ret->args = args;
    }
};
#endif //SYSYCOMPILER_FUNCPARAMSNODE_H
