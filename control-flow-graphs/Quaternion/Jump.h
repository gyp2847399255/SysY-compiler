//
// Created by gyp on 2021/10/30.
//

#ifndef SYSYCOMPILER_JUMP_H
#define SYSYCOMPILER_JUMP_H

class Jump : public Quaternion {
    BasicBlock *basicBlock;
public:
    explicit Jump(BasicBlock *basicBlock) : Quaternion(IRKind::JUMP), basicBlock(basicBlock) {}

    BasicBlock* getJumpBlock() override {
        return basicBlock;
    }

    void setJumpBlock(BasicBlock *block) override {
        this->basicBlock = block;
    }

    void print(ofstream *out) override {
        *out << "jump " << *basicBlock->getTag() << endl;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {}

    void assembly(ofstream *out, Function*) override {
        *out << "j " << *basicBlock->getTag() << endl;
    }
};
#endif //SYSYCOMPILER_JUMP_H
