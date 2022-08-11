//
// Created by gyp on 2021/10/22.
//

#ifndef SYSYCOMPILER_QUATERNION_H
#define SYSYCOMPILER_QUATERNION_H
#include <bits/stdc++.h>

enum class IRKind {
    LW,
    SW,
    LOAD_ADDRESS,
    CREATE_POINTER,
    ASSIGN,
    EQ_BIN,
    NEQ_BIN,
    GE_BIN,
    GT_BIN,
    LE_BIN,
    LT_BIN,
    PLUS_BIN,
    MINUS_BIN,
    MUL_BIN,
    DIV_BIN,
    MOD_BIN,
    NOT_UNI,
    NEGATE_UNI,
    JUMP_TRUE,
    JUMP_FALSE,
    JUMP,
    FUNC_CALL,
    PRINT_INT,
    PRINT_STR,
    RETURN,
    READ_CHAR,
    GETCHAR,
    PHI,
    LOAD_PARAM,
    PUSH_PARAM,
    GET_RETURN,
    END
};

typedef pair<string, string> HashOfQuaternion;

string generateStr() {
    static int cnt = 0;
    cnt++;
    return "str_" + to_string(cnt);
}

string generateIRVar(const string *name, int id) {
    return "var_" + (*name) + "_" + to_string(id);
}

string generateIRFunc(string *name) {
    return "func_" + (*name);
}

class BasicBlock;
class Function;

class Quaternion {
protected:
    static void loadImmediateNumber(ofstream *out, const string& reg, const string *integer) {
        if (abs(stoi(*integer)) < 32768) {
            *out << "addi " << reg << ", $0, " << *integer << endl;
        } else {
            *out << "li " << reg << ", " << *integer << endl;
        }
    }
    IRKind kind;
public:
    Quaternion *next;
    Quaternion *last;
    BasicBlock *belong;

    explicit Quaternion(IRKind kind) : kind(kind), next(nullptr), last(nullptr), belong(nullptr) {}

    virtual ~Quaternion() = default;

    IRKind getKind() {
        return this->kind;
    }

    virtual string* getDefine() {
        return nullptr;
    }

    virtual set<string*>* getUse() {
        return new set<string*>;
    }

    virtual void setJumpBlock(BasicBlock*) {}

    virtual void print(ofstream*) = 0;

    virtual void globalValueNumbering(map<string, int>*, map<string, string>*,
            map<IRKind, map<HashOfQuaternion, string>*>*, map<IRKind, set<HashOfQuaternion>*>*) = 0;

    virtual BasicBlock* getJumpBlock() {
        return nullptr;
    }

    virtual void assembly(ofstream*, Function*) = 0;
};
#endif //SYSYCOMPILER_QUATERNION_H