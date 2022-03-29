//
// Created by gyp on 2021/10/26.
//

#ifndef SYSYCOMPILER_CONTROLFLOWGRAPHBUILDER_H
#define SYSYCOMPILER_CONTROLFLOWGRAPHBUILDER_H
#include "Function.h"
using namespace std;

class ControlFlowGraphBuilder {
    set<Function*> funcs;
    Function* curFunc;
    BasicBlock *curBasicBlock;
    int basicBlockCnt;
    int tmpCnt;
public:
    ControlFlowGraphBuilder() : basicBlockCnt(0), tmpCnt(0), curBasicBlock(nullptr), curFunc(nullptr) {}

    void insert(Quaternion *quaternion) {
        curBasicBlock->insert(quaternion);
    }

    string tmpVar() {
        tmpCnt++;
        return "tmp_" + to_string(tmpCnt);
    }

    BasicBlock* newBasicBlock() {
        return new BasicBlock(++basicBlockCnt);
    }

    void changeFunction(Function *function) {
        curFunc = function;
        funcs.insert(function);
    }

    void changeCur(BasicBlock *newBasicBlock) {
        if (newBasicBlock == this->curBasicBlock) {
            return;
        }
        curBasicBlock = newBasicBlock;
        curFunc->addBasicBlock(newBasicBlock);
    }

    void simplify() {
        for (auto & i : funcs) {
            i->simplify();
        }
    }

    void staticSingleAssignment() {
        for (auto & i : funcs) {
            i->staticSingleAssignment();
        }
    }

    void optimize() {
        for (auto & i : funcs) {
            i->optimize();
        }
    }

    void print(ofstream *out) {
        for (auto & i : funcs) {
            i->print(out);
        }
    }

    void registerAllocate() {
        for (auto & i : funcs) {
            i->registerAllocate();
        }
    }

    void assembly(ofstream *out) {
        for (auto & i : funcs) {
            i->assembly(out);
        }
    }

    BasicBlock* getCur() {
        return this->curBasicBlock;
    }
} controlFlowGraphBuilder;
#endif //SYSYCOMPILER_CONTROLFLOWGRAPHBUILDER_H
