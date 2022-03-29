//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_NEGATE_H
#define SYSYCOMPILER_NEGATE_H
class Negate : public Quaternion {
    string assigned;
    string integer;
public:
    Negate(string *assigned, string *integer) : Quaternion(IRKind::NEGATE_UNI),
    assigned(*assigned), integer(*integer) {}

    void print(ofstream *out) override {
        *out << "negate " << assigned << " " << integer << endl;
    }

    string* getDefine() override {
        return &assigned;
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
        bool integerIsConst = exprResIsNumber(&integer);
        if (integerIsConst) {
            (*constValue)[assigned] = -stoi(integer);
            return;
        }
        HashOfQuaternion hash(integer, "");
        if ((*previous)[IRKind::NEGATE_UNI]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::NEGATE_UNI])[hash];
        } else {
            (*(*previous)[IRKind::NEGATE_UNI])[hash] = assigned;
            (*cur)[IRKind::NEGATE_UNI]->insert(hash);
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (integer[0] != '$') {
            *out << "lw $27, " << integer.substr(2) << "($sp)" << endl;
            integer = "$27";
        }
        if (assigned[0] == '$') {
            *out << "subu " << assigned << ", $0, " << integer << endl;
        } else {
            *out << "subu $27, $0, "<< integer << endl;
            *out << "sw $27, " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};

#endif //SYSYCOMPILER_NEGATE_H
