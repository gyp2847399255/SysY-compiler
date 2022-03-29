//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCDEFNODE_H
#define SYSYCOMPILER_FUNCDEFNODE_H
#include "FuncTypeNode.h"
#include "TokenNode.h"
using namespace std;

class FuncDefNode : public Node {
    void print(ofstream *out) override {
        bool isMain = false;
        if (children[1]->getKind() == SyntaxKind::MAIN_KW) {
            isMain = true;
        }
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::FUNC_TYPE) {
                ((FuncTypeNode*)i)->print(out, isMain);
            } else {
                i->print(out);
            }
        }
        if (isMain) {
            *out << "<MainFuncDef>" << endl;
        } else {
            *out << "<FuncDef>" << endl;
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        string *name;
        int line;
        bool inVoid = true;
        vector<int> params;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT || i->getKind() == SyntaxKind::MAIN_KW) {
                name = ((TokenNode*)i)->getContent();
                line = i->getFinishLine();
            } else if (i->getKind() == SyntaxKind::L_PAREN) {
                symbol.startBlock();
            } else if (i->getKind() == SyntaxKind::BLOCK) {
                if (!symbol.addFunc(&params, name, inVoid)) {
                    errorList.emplace_back(ErrorKind::REDEFINE_IDENT, line);
                }
                ctx->inVoidFunc = inVoid;
                ctx->afterFuncDef = true;
            }
            auto ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::FUNC_TYPE) {
                inVoid = ret0->isVoid;
            } else if (i->getKind() == SyntaxKind::FUNC_PARAMS) {
                params = ret0->args;
            } else if (i->getKind() == SyntaxKind::BLOCK) {
                ret->isReturn = ret0->isReturn;
            }
            delete ret0;
        }
        if (!ret->isReturn && !inVoid) {
            errorList.emplace_back(ErrorKind::INT_FUNC_RETURN_LACKED, this->finishLine);
        }
        ctx->inVoidFunc = false;
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        string *name;
        vector<string*> params;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT || i->getKind() == SyntaxKind::MAIN_KW) {
                name = ((TokenNode*)i)->getContent();
            } else if (i->getKind() == SyntaxKind::L_PAREN) {
                symbol.startBlock();
            } else if (i->getKind() == SyntaxKind::BLOCK) {
                symbol.addFunc(name);
                string funcName = generateIRFunc(name);
                controlFlowGraphBuilder.changeFunction(new Function(&funcName, params.size()));
                controlFlowGraphBuilder.changeCur(controlFlowGraphBuilder.newBasicBlock());
                for (int j = 0; j < params.size(); j++) {
                    controlFlowGraphBuilder.insert(new LoadParam(params[j], j));
                }
                ctx->afterFuncDef = true;
            }
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::FUNC_PARAMS) {
                params = ret->args;
            } else if (i->getKind() == SyntaxKind::BLOCK) {
                string tmp = "-";
                controlFlowGraphBuilder.insert(new Return(&tmp));
            }
        }
    }
};
#endif //SYSYCOMPILER_FUNCDEFNODE_H
