//
// Created by gyp on 2021/10/26.
//

#ifndef SYSYCOMPILER_ASSIGN_H
#define SYSYCOMPILER_ASSIGN_H
class Assign : public Quaternion{
    string assigned;
    string integer;
public:
    Assign(const string *assigned, const string *integer) : Quaternion(IRKind::ASSIGN),
    assigned(*assigned), integer(*integer) {}

    void print(ofstream *out) override {
        *out << "assign " << assigned << " " << integer << endl;
    }

    string* getDefine() override {
        return &assigned;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        if (!exprResIsNumber(&integer)) {
            ret->insert(&integer);
        }
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
        if (exprResIsNumber(&integer)) {
            (*constValue)[assigned] = stoi(integer);
        } else {
            (*copyValue)[assigned] = integer;
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (integer[0] == 's') {
            *out << "lw $27, " + integer.substr(2) + "($sp)" << endl;
            integer = "$27";
        }
        if (assigned[0] == '$') {
            if (exprResIsNumber(&integer)) {
                *out << "li " << assigned << ", " << integer << endl;
            } else {
                *out << "move " + assigned << ", " << integer << endl;
            }
        } else {
            if (exprResIsNumber((&integer))) {
                *out << "li $27, " << integer << endl;
                integer = "$27";
            }
            *out << "sw " + integer << ", " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_ASSIGN_H
