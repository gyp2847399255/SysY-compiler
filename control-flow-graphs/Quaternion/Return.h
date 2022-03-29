//
// Created by gyp on 2021/10/31.
//

#ifndef SYSYCOMPILER_RETURN_H
#define SYSYCOMPILER_RETURN_H
class Return : public Quaternion {
    string integer;
public:
    explicit Return(string *integer) : Quaternion(IRKind::RETURN),
    integer(*integer) {}

    void print(ofstream *out) override {
        *out << "return " << integer << endl;
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

    set<string*>* getUse() override {
        auto ret = new set<string*>();
        ret->insert(&integer);
        return ret;
    }

    void assembly(ofstream *out, Function *function) override;
};
#endif //SYSYCOMPILER_RETURN_H
