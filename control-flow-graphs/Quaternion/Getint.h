//
// Created by gyp on 2021/11/6.
//

#ifndef SYSYCOMPILER_GETINT_H
#define SYSYCOMPILER_GETINT_H
class Getint : public Quaternion {
    string assigned;
public:
    explicit Getint(string *assigned) : Quaternion(IRKind::GETINT),
    assigned(*assigned) {}

    void print(ofstream *out) override {
        *out << "getInt " << assigned << endl;
    }

    string* getDefine() override {
        return &assigned;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        if (assigned[0] == '$') {
            *out << "move " << assigned << ", $v0" << endl;
        } else {
            *out << "sw $v0," << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_GETINT_H
