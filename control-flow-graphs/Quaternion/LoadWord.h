//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_LOADWORD_H
#define SYSYCOMPILER_LOADWORD_H
class LoadWord : public Quaternion {
    string assigned;
    string pointer;
    string offset;
public:
    LoadWord(string *assigned, string *pointer, string *offset) : Quaternion(IRKind::LW),
    assigned(*assigned), pointer(*pointer), offset(*offset) {}

    void print(ofstream *out) override {
        *out << "loadWord " << assigned << " " << pointer << " " << offset << endl;
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
            *out << "add $28, $28, " << pointer << endl;
            pointer = "$28";
            offset = "0";
        }
        if (assigned[0] == '$') {
            *out << "lw " << assigned << ", " << offset << "(" << pointer << ")" << endl;
        } else {
            *out << "lw $27, " << offset << "(" << pointer << ")" << endl;
            *out << "sw $27, " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};

#endif //SYSYCOMPILER_LOADWORD_H
