//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_COMPILEUNITNODE_H
#define SYSYCOMPILER_COMPILEUNITNODE_H
#include "Node.h"
using namespace std;

class CompileUnitNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
        *out << "<CompUnit>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet * ret) override {
        symbol.startBlock();
        for (auto & i : children) {
            i->checkError(ctx, ret);
            ctx->isGlobal = false;
        }
        delete ctx;
        delete ret;
        symbol.finishBlock();
        symbol.clear();
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        symbol.startBlock();
        ctx->isGlobal = true;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::FUNC_DEF) {
                ctx->isGlobal = false;
            }
            i->buildIR(ctx, ret);
        }
        if (OPT) {
            controlFlowGraphBuilder.simplify();
            controlFlowGraphBuilder.staticSingleAssignment();
            controlFlowGraphBuilder.optimize();
        }
        if (IR) {
            ofstream dataFile("data.txt");
            symbol.printGlobalVar(&dataFile);
            ofstream irFile("ir.txt");
            controlFlowGraphBuilder.print(&irFile);
        } else {
            controlFlowGraphBuilder.registerAllocate();
            ofstream assemblyFile("mips.txt");
            assemblyFile << ".data" << endl;
            symbol.printGlobalVar(&assemblyFile);
            assemblyFile << ".text" << endl;
            assemblyFile << "jal func_main" << endl;
            assemblyFile << "ori $v0, $0, 10" << endl;
            assemblyFile << "syscall" << endl;
            controlFlowGraphBuilder.assembly(&assemblyFile);
        }
        delete ctx;
        delete ret;
        symbol.finishBlock();
    }
};
#endif //SYSYCOMPILER_COMPILEUNITNODE_H
