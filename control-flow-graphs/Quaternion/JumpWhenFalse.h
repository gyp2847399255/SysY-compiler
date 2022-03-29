//
// Created by gyp on 2021/10/29.
//

#ifndef SYSYCOMPILER_JUMPWHENFALSE_H
#define SYSYCOMPILER_JUMPWHENFALSE_H
class JumpWhenFalse : public Quaternion {
    BasicBlock *basicBlock;
    string integer;
public:
    JumpWhenFalse(BasicBlock *basicBlock, string *integer) : Quaternion(IRKind::JUMP_FALSE),
    basicBlock(basicBlock), integer(*integer) {}

    BasicBlock* getJumpBlock() override {
        return basicBlock;
    }

    void setJumpBlock(BasicBlock *block) override {
        this->basicBlock = block;
    }

    void print(ofstream *out) override {
        *out << "jumpWhenFalse " << *basicBlock->getTag() << " " << integer << endl;
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
        if (exprResIsNumber(&integer)) {
            if (stoi(integer) == 0) {
                *out << "j " << *basicBlock->getTag() << endl;
            }
            return;
        } else if (integer[0] != '$') {
            *out << "lw $27, " << integer.substr(2) << "($sp)" << endl;
            integer = "$27";
        }
        *out << "beqz " << integer << ", " << *basicBlock->getTag() << endl;
    }
};
#endif //SYSYCOMPILER_JUMPWHENFALSE_H
