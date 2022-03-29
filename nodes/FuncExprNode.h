//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_FUNCEXPRNODE_H
#define SYSYCOMPILER_FUNCEXPRNODE_H
#include "Node.h"
#include "TokenNode.h"
using namespace std;

class FuncExprNode : public Node {
    string res;
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        FuncSymbol *funcSymbol;
        bool flag = false;
        int line;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                funcSymbol = symbol.getFunc(((TokenNode*)i)->getContent());
                if (funcSymbol == nullptr) {
                    errorList.emplace_back(ErrorKind::UNDEFINED_IDENT, i->getFinishLine());
                } else if (funcSymbol->getVoid()) {
                    ret->dimension = -1;
                }
            }
            auto ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::IDENT) {
                line = ((TokenNode*)i)->getFinishLine();
            } else if (funcSymbol != nullptr &&
            (i->getKind() == SyntaxKind::FUNC_ARGS || (!flag && i->getKind() == SyntaxKind::R_PAREN))) {
                flag = true;
                ErrorKind kind = funcSymbol->matchParams(&ret0->args);
                if (kind == ErrorKind::PARAMS_NUM_UNMATCHED) {
                    errorList.emplace_back(ErrorKind::PARAMS_NUM_UNMATCHED, line);
                } else if (kind == ErrorKind::PARAM_TYPE_UNMATCHED) {
                    errorList.emplace_back(ErrorKind::PARAM_TYPE_UNMATCHED, line);
                }
            }
            delete ret0;
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        string *name;
        FuncSymbol *funcSymbol;
        vector<string*> args;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
                funcSymbol = symbol.getFunc(name);
            }
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::FUNC_ARGS) {
                args = ret->args;
            }
        }
        if (funcSymbol->getVoid()) {
            this->res = "-";
        } else {
            this->res = controlFlowGraphBuilder.tmpVar();
        }
        string irFunc = generateIRFunc(name);
        for (int i = 0; i < args.size(); i++) {
            controlFlowGraphBuilder.insert(new PushParam(args[i], i));
        }
        controlFlowGraphBuilder.insert(new FuncCall(&irFunc));
        if (!funcSymbol->getVoid()) {
            controlFlowGraphBuilder.insert(new GetReturn(&this->res));
        }
        ret->res = &this->res;
    }
};
#endif //SYSYCOMPILER_FUNCEXPRNODE_H
