//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_NOTLOGIC_H
#define SYSYCOMPILER_NOTLOGIC_H
class NotLogic : public Quaternion {
    string assigned;
    string integer;
public:
    NotLogic(string *assigned, string *integer) : Quaternion(IRKind::NOT_UNI),
    assigned(*assigned), integer(*integer) {}

    void print(ofstream *out) override {
        *out << "not " << assigned << " " << integer << endl;
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
            (*constValue)[assigned] = stoi(integer) == 0 ? 1 : 0;
            return;
        }
        HashOfQuaternion hash(integer, "");
        if ((*previous)[IRKind::NOT_UNI]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::NOT_UNI])[hash];
        } else {
            (*(*previous)[IRKind::NOT_UNI])[hash] = assigned;
            (*cur)[IRKind::NOT_UNI]->insert(hash);
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (integer[0] != '$') {
            *out << "lw $27, " << integer.substr(2) << "($sp)" << endl;
            integer = "$27";
        }
        if (assigned[0] == '$') {
            *out << "seq " << assigned << ", $0, " << integer << endl;
        } else {
            *out << "seq $27, $0, "<< integer << endl;
            *out << "sw $27, " << assigned.substr(2) << "($sp)" << endl;
        }
    }
};

#endif //SYSYCOMPILER_NOTLOGIC_H
