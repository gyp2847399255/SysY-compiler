//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_VARDEFNODE_H
#define SYSYCOMPILER_VARDEFNODE_H
#include "InitValNode.h"
#include "TokenNode.h"
using namespace std;

class VarDefNode : public Node {
public:
    void print(ofstream *out, bool isConst) {
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::EXPR) {
                ((ExprNode*)i)->print(out, ExprType::CONST_EXPR);
            } else if (i->getKind() == SyntaxKind::INIT_VAL) {
                ((InitValNode*)i)->print(out, isConst);
            } else {
                i->print(out);
            }
        }
        if (isConst) {
            *out << "<ConstDef>" << endl;
        } else {
            *out << "<VarDef>" << endl;
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        string *name;
        int line;
        vector<int> limit;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
                line = i->getFinishLine();
            }
            auto ret0 = new ErrorCheckRet();
            i->checkError(ctx, ret0);
            if (i->getKind() == SyntaxKind::EXPR) {
                limit.push_back(ret0->val);
            }
            delete ret0;
        }
        if (!symbol.addVar(ctx->isConst, &limit, name)) {
            errorList.emplace_back(ErrorKind::REDEFINE_IDENT, line);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        bool isConst = ctx->isConst;
        string *name;
        vector<int> dimension;
        vector<string*> init;
        vector<int> initVal;
        bool flag = isConst || symbol.getLayer() == 1;
        ret->clear();
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::IDENT) {
                name = ((TokenNode*)i)->getContent();
            }
            i->buildIR(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                dimension.push_back(stoi(*(ret->res)));
            } else if (i->getKind() == SyntaxKind::INIT_VAL) {
                if (flag) {
                    for (auto & j : ret->init) {
                        initVal.push_back(stoi(*j));
                    }
                } else {
                    init = ret->init;
                }
            }
        }
        int id = symbol.addVar(isConst, &dimension, name, &initVal);
        if (flag) {
            return;
        }
        if (!dimension.empty()) {
            string pointer = generateIRVar(name, id);
            int size = 1;
            for (auto & j : dimension) {
                size *= j;
            }
            controlFlowGraphBuilder.insert(new CreatePointer(&pointer, size));
            if (!init.empty()) {
                for (int i = 0; i < size; i++) {
                    string offset = to_string(i);
                    controlFlowGraphBuilder.insert(new StoreWord(init[i], &pointer, &offset));
                }
            }
        } else {
            string var = generateIRVar(name, id);
            if (!init.empty()) {
                controlFlowGraphBuilder.insert(new Assign(&var, init[0]));
            } else {
                string tmp = "19260817";
                controlFlowGraphBuilder.insert(new Assign(&var, &tmp));
            }
        }
    }
};
#endif //SYSYCOMPILER_VARDEFNODE_H
