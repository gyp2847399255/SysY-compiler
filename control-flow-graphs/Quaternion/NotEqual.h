//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_NOTEQUAL_H
#define SYSYCOMPILER_NOTEQUAL_H
class NotEqual : public Binary {
public:
    NotEqual(string *assigned, string *integer1, string *integer2) :
    Binary(IRKind::NEQ_BIN, assigned, integer1, integer2) {}

    void print(ofstream *out) override {
        *out << "notEqual " << assigned << " " << integer1 << " " << integer2 << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        if (dealUsing(constValue, copyValue)) {
            return;
        }
        bool integer1IsConst = exprResIsNumber(&integer1);
        bool integer2IsConst = exprResIsNumber(&integer2);
        if (integer1IsConst && integer2IsConst) {
            (*constValue)[assigned] = stoi(integer1) != stoi(integer2);
            return;
        } else if (integer1 == integer2) {
            (*constValue)[assigned] = 0;
            return;
        }
        if (integer1 > integer2) {
            swap(integer1, integer2);
        }
        HashOfQuaternion hash(integer1, integer2);
        if ((*previous)[IRKind::NEQ_BIN]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::NEQ_BIN])[hash];
        } else {
            (*(*previous)[IRKind::NEQ_BIN])[hash] = assigned;
            (*cur)[IRKind::NEQ_BIN]->insert(hash);
        }
    }

    void assembly(ofstream *out, Function*) override {
        if (exprResIsNumber(&integer1)) {
            *out << "li $27, " + integer1 << endl;
            integer1 = "$27";
        } else if (integer1[0] != '$') {
            *out << "lw $27, " + integer1.substr(2) + "($sp)" << endl;
            integer1 = "$27";
        }
        if (integer2[0] == 's') {
            *out << "lw $28, " + integer2.substr(2) << "($sp)" << endl;
            integer2 = "$28";
        }
        if (assigned[0] == '$') {
            *out << "sne " << assigned << ", " << integer1 << ", " << integer2 << endl;
        } else {
            *out << "sne $27, " << integer1 << ", " << integer2 << endl;
            *out << "sw $27," << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_NOTEQUAL_H
