//
// Created by gyp on 2021/10/25.
//

#ifndef SYSYCOMPILER_CONTEXT_H
#define SYSYCOMPILER_CONTEXT_H
#include "../control-flow-graphs/BasicBlock.h"

struct ErrorCheckRet {
    int val;
    int dimension; // defined in params, calculated in expression
    bool isConst, isReturn, isVoid;
    vector<int> args{}; // dimensions of each param and argument
    ErrorCheckRet() : val(0), dimension(0), isConst(false), isReturn(false), isVoid(false) {}
};

struct ErrorCheckCtx {
    bool inVoidFunc, afterFuncDef, isConst, isLVal, isGlobal;
    int loopNum;
    ErrorCheckCtx() : isConst(false), isLVal(false), isGlobal(false),
    afterFuncDef(false), inVoidFunc(false), loopNum(0) {}
};

struct BuildIRRet {
    string *res; // stored in exprNode
    string *param;
    vector<string*> args;
    vector<string*> init;
    bool isArray;
    BuildIRRet() : res(nullptr), param(nullptr), isArray(false) {}

    void clear() {
        param = nullptr;
        args.clear();
        init.clear();
    }

    ~BuildIRRet() {
        delete param;
    }
};

struct ExprNode;
struct BuildIRCtx {
    bool isConst, afterFuncDef, isLVal, isGlobal, needJumpElse, stmtAfterLoop;
    string *res;
    ExprNode *condition;
    BasicBlock *continueBlock, *breakBlock, *elseFinalBlock;
    BuildIRCtx() : continueBlock(nullptr), breakBlock(nullptr), elseFinalBlock(nullptr), res(nullptr),
    condition(nullptr), isConst(false), afterFuncDef(false), isLVal(false), isGlobal(false),
    needJumpElse(false), stmtAfterLoop(false) {}
};

#endif //SYSYCOMPILER_CONTEXT_H