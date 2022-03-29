//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_PREFIXOPNODE_H
#define SYSYCOMPILER_PREFIXOPNODE_H
#include "Node.h"
using namespace std;

class PrefixOPNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
            *out << "<UnaryOp>" << endl;
        }
    }
    int getSize() {
        return children.size();
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        if (ctx->isConst) {
            int val = stoi(*ctx->res);
            for (int i = children.size() - 1; i >= 0; i--) {
                switch (children[i]->getKind()) {
                    case SyntaxKind::NOT_LOGIC:
                        val = !val;
                        break;
                    case SyntaxKind::MINUS:
                        val = -val;
                    default:
                        break;
                }
            }
            *(ctx->res) = to_string(val);
            return;
        }
        string tmp;
        for (int i = children.size() - 1; i >= 0; i--) {
            tmp = controlFlowGraphBuilder.tmpVar();
            switch (children[i]->getKind()) {
                case SyntaxKind::NOT_LOGIC:
                    controlFlowGraphBuilder.insert(new NotLogic(&tmp, ctx->res));
                    *ctx->res = tmp;
                    break;
                case SyntaxKind::MINUS:
                    controlFlowGraphBuilder.insert(new Negate(&tmp, ctx->res));
                    *ctx->res = tmp;
                    break;
                default:
                    break;
            }
        }
    }
};
#endif //SYSYCOMPILER_PREFIXOPNODE_H
