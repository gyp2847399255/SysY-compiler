//
// Created by gyp on 2021/10/27.
//

#ifndef SYSYCOMPILER_PRINTINTEGER_H
#define SYSYCOMPILER_PRINTINTEGER_H
class PrintInteger : public Quaternion {
    string integer;
public:
    explicit PrintInteger(string *integer) : Quaternion(IRKind::PRINT_INT),
    integer(*integer) {}

    void print(ofstream *out) override {
        *out << "printInteger " << integer << endl;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        ret->insert(&integer);
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
    }

    void assembly(ofstream *out, Function*) override {
        if (integer[0] == '$') {
            *out << "move $a0," << integer << endl;
        } else if (integer[0] == 's') {
            *out << "lw $a0, " << integer.substr(2) << "($sp)" << endl;
        } else {
            *out << "li $a0, " << integer << endl;
        }
        *out << "li $v0, 1" << endl;
        *out << "syscall" << endl;
    }
};
#endif //SYSYCOMPILER_PRINTINTEGER_H
