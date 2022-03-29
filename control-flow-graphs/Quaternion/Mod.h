//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_MOD_H
#define SYSYCOMPILER_MOD_H

class Mod : public Binary {
public:
    Mod(string *assigned, string *integer1, string *integer2) :
    Binary(IRKind::MOD_BIN, assigned, integer1, integer2) {}

    void print(ofstream *out) override {
        *out << "mod " << assigned << " " << integer1 << " " << integer2 << endl;
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
            (*constValue)[assigned] = stoi(integer1) % stoi(integer2);
            return;
        } else if (integer1 == integer2) {
            (*constValue)[assigned] = 0;
            return;
        } else if (integer2IsConst && stoi(integer2) == 1) {
            (*constValue)[assigned] = 0;
            return;
        }
        HashOfQuaternion hash(integer1, integer2);
        if ((*previous)[IRKind::MOD_BIN]->count(hash)) {
            (*copyValue)[assigned] = (*(*previous)[IRKind::MOD_BIN])[hash];
        } else {
            (*(*previous)[IRKind::MOD_BIN])[hash] = assigned;
            (*cur)[IRKind::MOD_BIN]->insert(hash);
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
            *out << "slt $ra, " << integer1 << ", $0" << endl;
            *out << "addu $28, $28, $ra" << endl;
            *out << "xori $28, $28, " << d << endl;
            *out << "subiu $28, $28, " << d << endl;
            *out << "mul $28, $28, " << integer2 << endl;
            if (assigned[0] == '$') {
                *out << "subu " << assigned << ", " << integer1 << ", $28" << endl;
            } else {
                *out << "subu $28, " << integer1 << ", $28" << endl;
                *out << "sw $28, " << assigned.substr(2) << "($sp)" << endl;
            }
            return;
        } else if (integer2[0] != '$') {
            *out << "lw $28, " + integer2.substr(2) << "($sp)" << endl;
            integer2 = "$28";
        }
        *out << "div " + integer1 << ", " << integer2 << endl;
        if (assigned[0] == '$') {
            *out << "mfhi " << assigned << endl;
        } else {
            *out << "mfhi $28" << endl;
            *out << "sw $28," << assigned.substr(2) << "($sp)" << endl;
        }
    }
};
#endif //SYSYCOMPILER_MOD_H
