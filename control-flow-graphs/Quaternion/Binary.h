//
// Created by gyp on 2021/11/20.
//

#ifndef SYSYCOMPILER_BINARY_H
#define SYSYCOMPILER_BINARY_H

class Binary : public Quaternion {
protected:
    string integer1;
    string integer2;
    string assigned;
    bool dealUsing(map<string, int> *constValue, map<string, string> *copyValue) {
        if (constValue->count(integer1)) {
            integer1 = to_string((*constValue)[integer1]);
        } else if (copyValue->count(integer1)) {
            integer1 = (*copyValue)[integer1];
        }
        if (constValue->count(integer2)) {
            integer2 = to_string((*constValue)[integer2]);
        } else if (copyValue->count(integer2)) {
            integer2 = (*copyValue)[integer2];
        }
        return false;
    }
public:
    Binary(IRKind kind, string *assigned, string *integer1, string *integer2) : Quaternion(kind),
    integer1(*integer1), integer2(*integer2), assigned(*assigned) {}

    string* getDefine() override {
        return &assigned;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        if (!exprResIsNumber(&integer1)) {
            ret->insert(&integer1);
        }
        if (!exprResIsNumber(&integer2)) {
            ret->insert(&integer2);
        }
        return ret;
    }
};
#endif //SYSYCOMPILER_BINARY_H
