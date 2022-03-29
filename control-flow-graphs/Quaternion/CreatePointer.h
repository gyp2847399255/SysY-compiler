//
// Created by gyp on 2021/10/26.
//

#ifndef SYSYCOMPILER_CREATEPOINTER_H
#define SYSYCOMPILER_CREATEPOINTER_H

class CreatePointer : public Quaternion {
    string name;
    int size;
public:
    CreatePointer(string *name, int size) :
    Quaternion(IRKind::CREATE_POINTER), name(*name), size(size) {}

    string* getDefine() override {
        return &name;
    }

    int getSize() const {
        return size;
    }

    void print(ofstream *out) override {
        *out << "createPointer " << name << " " << size << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream*, Function*) override {}
};
#endif //SYSYCOMPILER_CREATEPOINTER_H
