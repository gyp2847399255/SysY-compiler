//
// Created by gyp on 2021/10/30.
//

#ifndef SYSYCOMPILER_LOADPOINTER_H
#define SYSYCOMPILER_LOADPOINTER_H
class LoadPointer : public Quaternion {
    string assigned;
    string pointer;
    string offset;
public:
    LoadPointer(string *assigned, string *pointer, string *offset) : Quaternion(IRKind::LOAD_ADDRESS),
    assigned(*assigned), pointer(*pointer), offset(*offset) {}

    void print(ofstream *out) override {
        *out << "loadPointer " << assigned << " " << pointer << " " << offset << endl;
    }

    string* getDefine() override {
        return &assigned;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        ret->insert(&offset);
        ret->insert(&pointer);
        return ret;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        if (constValue->count(offset)) {
            offset = to_string((*constValue)[offset]);
        } else if (copyValue->count(offset)) {
            offset = (*copyValue)[offset];
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (pointer[0] == 'a') {
            *out << "addi $27, $sp, " << pointer.substr(5) << endl;
            pointer = "$27";
        } else if (pointer[0] == 'v') {
            *out << "la $27, " << pointer << endl;
            pointer = "$27";
        } else if (pointer[0] == 's') {
            *out << "lw $27, " << pointer.substr(2) << "($sp)" << endl;
            pointer = "$27";
        }
        if (exprResIsNumber(&offset)) {
            offset = to_string(stoi(offset) * 4);
        } else {
            if (offset[0] != '$') {
                *out << "lw $28, " << offset.substr(2) << "($sp)" << endl;
                offset = "$28";
            }
            *out << "sll $28, " << offset << ", 2" << endl;
            offset = "$28";
        }
        if (assigned[0] == '$') {
            *out << "add " << assigned << ", " << pointer << ", " << offset << endl;
        } else {
            *out << "add $27, " << pointer << ", " << offset << endl;
            *out << "sw $27, " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_LOADPOINTER_H
