//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_GREATEROREQUAL_H
#define SYSYCOMPILER_GREATEROREQUAL_H

class GreaterOrEqual : public Binary {
public:
    GreaterOrEqual(string *assigned, string *integer1, string *integer2) :
    Binary(IRKind::GE_BIN, assigned, integer1, integer2) {}

    void print(ofstream *out) override {
        *out << "greatEqual " << assigned << " " << integer1 << " " << integer2 << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        dealUsing(constValue, copyValue);
        if (exprResIsNumber(&integer1) && exprResIsNumber(&integer2)) {
            (*constValue)[assigned] = stoi(integer1) >= stoi(integer2) ? 1 : 0;
            return;
        } else if (integer1 == integer2) {
            (*constValue)[assigned] = 1;
            return;
        }
        HashOfQuaternion hash(integer1, integer2);
        if ((*previous)[IRKind::GE_BIN]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::GE_BIN])[hash];
        } else {
            (*(*previous)[IRKind::GE_BIN])[hash] = assigned;
            (*cur)[IRKind::GE_BIN]->insert(hash);
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
        if (!exprResIsNumber(&integer2) && integer2[0] != '$') {
            *out << "lw $28, " + integer2.substr(2) << "($sp)" << endl;
            integer2 = "$28";
        }
        if (assigned[0] == '$') {
            *out << "sge " << assigned << ", " << integer1 << ", " << integer2 << endl;
        } else {
            *out << "sge $27, " << integer1 << ", " << integer2 << endl;
            *out << "sw $27," << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_GREATEROREQUAL_H
