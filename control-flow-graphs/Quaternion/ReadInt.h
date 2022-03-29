//
// Created by gyp on 2021/11/16.
//

#ifndef SYSYCOMPILER_READINT_H
#define SYSYCOMPILER_READINT_H

class ReadInt : public Quaternion {
public:
    ReadInt() : Quaternion(IRKind::READ_INT) {}

    void print(ofstream *out) override {
        *out << "readInt" << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        *out << "li $v0, 5" << endl;
        *out << "syscall" << endl;
    }
};
#endif //SYSYCOMPILER_READINT_H
