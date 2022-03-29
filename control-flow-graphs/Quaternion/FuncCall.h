//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_FUNCCALL_H
#define SYSYCOMPILER_FUNCCALL_H
#include "Quaternion.h"

class FuncCall : public Quaternion {
    string name;
public:
    explicit FuncCall(string *name) : Quaternion(IRKind::FUNC_CALL),
    name(*name) {}

    void print(ofstream *out) override {
        *out << "funcCall " << this->name << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        *out << "jal " << name << endl;
    }
};

#endif //SYSYCOMPILER_FUNCCALL_H
