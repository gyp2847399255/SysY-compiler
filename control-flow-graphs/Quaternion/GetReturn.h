//
// Created by gyp on 2021/11/16.
//

#ifndef SYSYCOMPILER_GETRETURN_H
#define SYSYCOMPILER_GETRETURN_H

class GetReturn : public Quaternion {
    string assigned;
public:
    explicit GetReturn(string *assigned) : Quaternion(IRKind::GET_RETURN), assigned(*assigned) {}

    string* getDefine() override {
        return &assigned;
    }

    void print(ofstream *out) override {
        *out << "getReturn " << assigned << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        if (assigned[0] == '$') {
            *out << "move " << assigned << ", $v0" << endl;
        } else {
            *out << "sw $v0, " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_GETRETURN_H
