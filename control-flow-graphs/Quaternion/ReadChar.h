//
// Created by gyp on 2021/11/16.
//

#ifndef SYSYCOMPILER_READCHAR_H
#define SYSYCOMPILER_READCHAR_H

class ReadChar : public Quaternion {
public:
    ReadChar() : Quaternion(IRKind::READ_CHAR) {}

    void print(ofstream *out) override {
        *out << "readChar" << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        *out << "li $v0, 12" << endl;
        *out << "syscall" << endl;
    }
};
#endif //SYSYCOMPILER_READCHAR_H
