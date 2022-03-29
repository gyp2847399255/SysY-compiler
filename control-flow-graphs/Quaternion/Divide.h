//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_DIVIDE_H
#define SYSYCOMPILER_DIVIDE_H

class Divide : public Binary {
public:
    Divide(string *assigned, string *integer1, string *integer2) :
    Binary(IRKind::DIV_BIN, assigned, integer1, integer2) {}

    void print(ofstream *out) override {
        *out << "divide " << assigned << " " << integer1 << " " << integer2 << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        if (dealUsing(constValue, copyValue)) {
            return;
        }
        if (exprResIsNumber(&integer2)) {
            if (exprResIsNumber(&integer1)) {
                (*constValue)[assigned] = stoi(integer1) / stoi(integer2);
                return;
            }
            if (stoi(integer2) == 1) {
                (*copyValue)[assigned] = integer1;
                return;
            }
        } else if (integer1 == integer2) {
            (*constValue)[assigned] = 1;
            return;
        }
        HashOfQuaternion hash(integer1, integer2);
        if ((*previous)[IRKind::DIV_BIN]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::DIV_BIN])[hash];
        } else {
            (*(*previous)[IRKind::DIV_BIN])[hash] = assigned;
            (*cur)[IRKind::DIV_BIN]->insert(hash);
        }
    }

    void assembly(ofstream *out, Function *function) override {
        if (exprResIsNumber(&integer1)) {
            *out << "li $27, " + integer1 << endl;
            integer1 = "$27";
        } else if (integer1[0] != '$') {
            *out << "lw $27, " + integer1.substr(2) + "($sp)" << endl;
            integer1 = "$27";
        }
        if (exprResIsNumber(&integer2)) {
            long long op2 = stoi(integer2);
            int l = max((int) ceil(log2(abs(op2))), 1);
            unsigned long long m = 1 + (1ll << (32 + l - 1)) / abs(op2);
            int m2 = m - (1ll << 32);
            int d = op2 < 0 ? -1 : 0;
            int sh = l - 1;
            string int2 = to_string(m2);
            *out << "li $28, " << m2 << endl;
            *out << "mult $28, " << integer1 << endl;
            *out << "mfhi $28" << endl;
            *out << "addu $28, " << integer1 << ", $28" << endl;
            *out << "sra $28, $28, " << sh << endl;
            *out << "slt $27, " << integer1 << ", $0" << endl;
            *out << "addu $28, $28, $27" << endl;
            *out << "xori $28, $28, " << d << endl;
            if (assigned[0] == '$') {
                *out << "subiu " << assigned << " $28, " << d << endl;
            } else {
                *out << "subiu $28, $28, " << d << endl;
                *out << "sw $28, " << assigned.substr(2) << "($sp)" << endl;
            }
            return;
        } else if (integer2[0] != '$') {
            *out << "lw $28, " + integer2.substr(2) << "($sp)" << endl;
            integer2 = "$28";
        }

        *out << "div " + integer1 << ", " << integer2 << endl;
        if (assigned[0] == '$') {
            *out << "mflo " << assigned << endl;
        } else {
            *out << "mflo $28" << endl;
            *out << "sw $28," << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_DIVIDE_H
