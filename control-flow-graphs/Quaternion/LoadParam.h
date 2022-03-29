//
// Created by gyp on 2021/11/15.
//

#ifndef SYSYCOMPILER_LOADPARAM_H
#define SYSYCOMPILER_LOADPARAM_H
class LoadParam : public Quaternion {
    string integer;
    int num;
public:
    LoadParam(string *param, int num) : Quaternion(IRKind::LOAD_PARAM),
    integer(*param), num(num) {}

    string* getDefine() override {
        return &integer;
    }

    void print(ofstream *out) override {
        *out << "pop " << integer << " " << num << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function *function) override;
};
#endif //SYSYCOMPILER_LOADPARAM_H
