//
// Created by gyp on 2021/10/27.
//

#ifndef SYSYCOMPILER_PRINTSTRING_H
#define SYSYCOMPILER_PRINTSTRING_H

class PrintString : public Quaternion {
    string str;
public:
    explicit PrintString(string *str) : Quaternion(IRKind::PRINT_STR),
    str(move(*str)) {}

    void print(ofstream *out) override {
        *out << "printString " << str << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        *out << "la $a0, " << str << endl;
        *out << "li $v0, 4" << endl;
        *out << "syscall" << endl;
    }
};
#endif //SYSYCOMPILER_PRINTSTRING_H
