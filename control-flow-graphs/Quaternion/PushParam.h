//
// Created by gyp on 2021/11/15.
//

#ifndef SYSYCOMPILER_PUSHPARAM_H
#define SYSYCOMPILER_PUSHPARAM_H
class PushParam : public Quaternion {
    string integer;
    int num;
public:
    PushParam(string *param, int num) : Quaternion(IRKind::PUSH_PARAM),
    integer(*param), num(num) {}

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        ret->insert(&integer);
        return ret;
    }

    void print(ofstream *out) override {
        *out << "push " << integer << " " << num << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        if (constValue->count(integer)) {
            integer = to_string((*constValue)[integer]);
        } else if (copyValue->count(integer)) {
            integer = (*copyValue)[integer];
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (num < 4) {
            if (integer[0] == '$') {
                *out << "move " << "$a" << num << ", " << integer << endl;
            } else if (integer[0] == 's') {
                *out << "lw $a" << num << ", " << integer.substr(2) << "($sp)" << endl;
            } else if (integer[0] == 'a') {
                *out << "addi $a" << num << ", $sp, " << integer.substr(5) << endl;
            } else {
                *out << "li $a" << num << ", " << integer << endl;
            }
        } else {
            if (integer[0] == '$') {
                *out << "sw " << integer << ", " << -(num - 3) * 4 << "($sp)" << endl;
            } else {
                if (integer[0] == 's') {
                    *out << "lw $27, " << integer.substr(2) << "($sp)" << endl;
                } else if (integer[0] == 'a') {
                    *out << "addi $27" << num << ", $sp, " << integer.substr(5) << endl;
                } else {
                    *out << "li $27, " << integer << endl;
                }
                *out << "sw $27, " << -(num - 3) * 4 << "($sp)" << endl;
            }
        }
    }
};
#endif //SYSYCOMPILER_PUSHPARAM_H
