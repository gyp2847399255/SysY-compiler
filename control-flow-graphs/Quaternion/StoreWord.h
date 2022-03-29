//
// Created by gyp on 2021/10/26.
//

#ifndef SYSYCOMPILER_STOREWORD_H
#define SYSYCOMPILER_STOREWORD_H

class StoreWord : public Quaternion {
    string integer;
    string pointer;
    string offset;
public:
    StoreWord(string *integer, string *pointer, string *offset) : Quaternion(IRKind::SW),
    integer(*integer), pointer(*pointer), offset(*offset) {}

    void print(ofstream *out) override {
        *out << "storeWord " << integer << " " << pointer << " " << offset << endl;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        ret->insert(&integer);
        ret->insert(&offset);
        ret->insert(&pointer);
        return ret;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        if (constValue->count(integer)) {
            integer = to_string((*constValue)[integer]);
        } else if (copyValue->count(integer)) {
            integer = (*copyValue)[integer];
        }
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
            *out << "add $27, $28, " << pointer << endl;
            pointer = "$27";
            offset = "0";
        }
        if (integer[0] == '$') {
            *out << "sw " << integer << ", " << offset << "(" << pointer << ")" << endl;
        } else {
            if (integer[0] == 's') {
                *out << "lw $28, " << integer.substr(2) << "($sp)" << endl;
            } else {
                *out << "li $28, " << integer << endl;
            }
            *out << "sw $28, " << offset << "(" << pointer << ")" << endl;
        }
    }
};
#endif //SYSYCOMPILER_STOREWORD_H
