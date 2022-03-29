//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_RETURNSTMTNODE_H
#define SYSYCOMPILER_RETURNSTMTNODE_H
#include "Node.h"
using namespace std;

class ReturnStmtNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        int line;
        for (auto & i : children) {
            i->checkError(ctx, ret);
            if (i->getKind() == SyntaxKind::RETURN_KW) {
                line = i->getStartLine();
            } else if (i->getKind() == SyntaxKind::EXPR && ctx->inVoidFunc) {
                errorList.emplace_back(ErrorKind::VOID_FUNC_RETURN_INTEGER, line);
            }
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        string *res = nullptr;
        for (auto & i : children) {
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                res = ret->res;
            }
        }
        if (res == nullptr) {
            string tmp = "-";
            controlFlowGraphBuilder.insert(new Return(&tmp));
        } else {
            controlFlowGraphBuilder.insert(new Return(res));
        }
    }
};
#endif //SYSYCOMPILER_RETURNSTMTNODE_H
