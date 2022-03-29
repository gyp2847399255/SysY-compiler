//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCTYPENODE_H
#define SYSYCOMPILER_FUNCTYPENODE_H
#include "Node.h"
using namespace std;

class FuncTypeNode : public Node {
public:
    void print(ofstream *out, bool isMain) {
        for (auto & i : children) {
            i->print(out);
        }
        if (!isMain)
            *out << "<FuncType>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        if (children[0]->getKind() == SyntaxKind::VOID_KW) {
            ret->isVoid = true;
        }
    }
};
#endif //SYSYCOMPILER_FUNCTYPENODE_H
