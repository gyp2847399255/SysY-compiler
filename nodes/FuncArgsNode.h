//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCARGSNODE_H
#define SYSYCOMPILER_FUNCARGSNODE_H
#include "Node.h"
using namespace std;

class FuncArgsNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
        *out << "<FuncRParams>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        for (auto & i : children) {
            auto *ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::EXPR) {
                ret->args.push_back(ret0->dimension);
            }
            delete ret0;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        vector<string *>args;
        for (auto & i : children) {
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                args.push_back(ret->res);
            }
        }
        ret->args = args;
    }
};
#endif //SYSYCOMPILER_FUNCARGSNODE_H
