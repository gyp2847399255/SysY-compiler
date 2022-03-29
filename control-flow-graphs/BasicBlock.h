//
// Created by gyp on 2021/10/22.
//

#ifndef SYSYCOMPILER_BASICBLOCK_H
#define SYSYCOMPILER_BASICBLOCK_H

class BasicBlock {
    string tag;
    Quaternion *head;
    Quaternion *tail;
    set<BasicBlock*> predecessor;
    set<BasicBlock*> successor;
    set<BasicBlock*> dominator;
    BasicBlock *immediateDominator;
    vector<BasicBlock*> dominateChildren;
    map<string, PhiFunction*> phiNode;
    set<string> activenessIn;
    set<string> activenessOut;
    set<string> variableDefine;
    set<string> variableUse;
    int dominateSize;
    int dfsNumber;
public:
    BasicBlock *next;
    BasicBlock *last;

    explicit BasicBlock(int id) : tag("label_" + to_string(id)), head(nullptr), tail(nullptr),
    next(nullptr), last(nullptr), immediateDominator(nullptr), dominateSize(0), dfsNumber(0) {}

    BasicBlock(BasicBlock *from, BasicBlock *to) : tag(from->tag + "_" + to->tag), head(nullptr),
    tail(nullptr), next(nullptr), last(nullptr), immediateDominator(nullptr), dominateSize(0), dfsNumber(0) {}

    void addPhi(const string *assigned) {
        phiNode[*assigned] = new PhiFunction(assigned);
    }

    void insertDefine(Quaternion *quaternion) {
        IRKind kind = quaternion->getKind();
        if (kind == IRKind::CREATE_POINTER) {
            return;
        }
        auto tmp = quaternion->getDefine();
        if (tmp != nullptr) {
            variableDefine.insert(*tmp);
        }
    }

    void insertUse(Quaternion *quaternion) {
        if (quaternion->getKind() == IRKind::PHI) {
            return;
        }
        auto use = quaternion->getUse();
        for (auto & i : *use) {
            variableUse.insert(*i);
        }
        delete use;
    }

    void addOutVariable(const string *var) {
        if (!activenessOut.count(*var)) {
            activenessOut.insert(*var);
            if (!variableDefine.count(*var) && !activenessIn.count(*var)) {
                activenessIn.insert(*var);
                for (auto & i : predecessor) {
                    i->addOutVariable(var);
                }
            }
        }
    }

    void getOutVariable() {
        for (auto & i : variableUse) {
            if (!variableDefine.count(i) && !activenessIn.count(i)) {
                activenessIn.insert(i);
                for (auto & j : predecessor) {
                    j->addOutVariable(&i);
                }
            }
        }
        for (auto & j : successor) {
            for (Quaternion *i = j->head; i && i->getKind() == IRKind::PHI; i = i->next) {
                addOutVariable(((PhiFunction*)i)->getItem(this));
            }
        }
    }

    Quaternion* getHead() {
        return head;
    }

    Quaternion* getTail() {
        return tail;
    }

    set<BasicBlock*>* getSuccessor() {
        return &successor;
    }

    void addSuccessor(BasicBlock *basicBlock) {
        successor.insert(basicBlock);
    }

    set<BasicBlock*> *getPredecessor() {
        return &predecessor;
    }

    bool dominates(BasicBlock *basicBlock) const {
        return basicBlock->dfsNumber >= this->dfsNumber &&
        basicBlock->dfsNumber < this->dfsNumber + this->dominateSize;
    }

    set<BasicBlock*>* getDominatorFrontier() {
        return &dominator;
    }

    void addDominatorFrontier(BasicBlock *basicBlock) {
        dominator.insert(basicBlock);
    }

    void addDominator(BasicBlock *basicBlock) {
        dominator.insert(basicBlock);
        basicBlock->dominateSize++;
    }

    void calcImmediateDominator() {
        unsigned int size = 0;
        for (auto & i : dominator) {
            if (i == this) {
                continue;
            }
            if (i->dominator.size() > size) {
                immediateDominator = i;
                size = i->dominator.size();
            }
        }
        if (immediateDominator != nullptr) {
            immediateDominator->dominateChildren.push_back(this);
        }
    }

    void dfsDominanceTree(int *cnt) {
        (*cnt)++;
        dominator.clear();
        dfsNumber = *cnt;
        for (auto & i : dominateChildren) {
            i->dfsDominanceTree(cnt);
        }
    }

    void addPredecessor(BasicBlock *basicBlock) {
        predecessor.insert(basicBlock);
    }

    BasicBlock* getJumpBlock() {
        if (tail == nullptr) {
            return nullptr;
        }
        return tail->getJumpBlock();
    }

    void setJumpBlock(BasicBlock *basicBlock) {
        tail->setJumpBlock(basicBlock);
    }

    void insert(Quaternion* quaternion) {
        quaternion->belong = this;
        if (tail == nullptr) {
            head = tail = quaternion;
        } else {
            quaternion->last = tail;
            tail->next = quaternion;
            tail = quaternion;
        }
    }

    void insertToFront(Quaternion* quaternion) {
        quaternion->belong = this;
        if (head == nullptr) {
            head = tail = quaternion;
        } else {
            quaternion->next = head;
            head->last = quaternion;
            head = quaternion;
        }
    }

    void insertPhiToList() {
        for (auto & i : phiNode) {
            insertToFront(i.second);
        }
        phiNode.clear();
        dominator.clear();
    }

    void eliminateQuaternion(Quaternion *quaternion) {
        if (quaternion->last == nullptr) {
            head = quaternion->next;
        } else {
            quaternion->last->next = quaternion->next;
        }
        if (quaternion->next == nullptr) {
            tail = quaternion->last;
        } else {
            quaternion->next->last = quaternion->last;
        }
    }

    string* getTag() {
        return &tag;
    }

    void simplify() {
        for (Quaternion *i = head; i; i = i->next) {
            IRKind kind = i->getKind();
            if (kind == IRKind::RETURN || i->getJumpBlock() != nullptr) {
                i->next = nullptr;
                tail = i;
            }
        }
    }

    void print(ofstream *out) {
        *out << tag << " : ";
        *out << endl;
        for (auto & i : phiNode) {
            i.second->print(out);
        }
        for (Quaternion *i = head; i; i = i->next) {
            i->print(out);
        }
    }

    bool accessToNextBlock() {
        return tail == nullptr || (tail->getKind() != IRKind::JUMP && tail->getKind() != IRKind::RETURN);
    }

    void rename(map<string, int> *variableInRegister, map<string, int> *variableInMemory,
                map<string, int> *arrayInMemory) {
        for (Quaternion *i = tail; i; i = i->last) {
            auto define = i->getDefine();
            if (define != nullptr) {
                if (variableInRegister->count(*define)) {
                    *define = "$" + to_string((*variableInRegister)[*define]);
                } else if (variableInMemory->count(*define)) {
                    *define = "sp" + to_string((*variableInMemory)[*define]);
                } else if (arrayInMemory->count(*define)) {
                    *define = "array" + to_string((*arrayInMemory)[*define]);
                }
            }
            auto use = i->getUse();
            for (auto & j : *use) {
                if (variableInRegister->count(*j)) {
                    *j = "$" + to_string((*variableInRegister)[*j]);
                } else if (variableInMemory->count(*j)) {
                    *j = "sp" + to_string((*variableInMemory)[*j]);
                } else if (arrayInMemory->count(*j)) {
                    *j = "array" + to_string((*arrayInMemory)[*j]);
                }
            }
            delete use;
        }
    }

    void assembly(ofstream *out, Function *function) {
        *out << tag << ":" << endl;
        for (Quaternion *i = head; i; i = i->next) {
            *out << "# ";
            i->print(out);
            i->assembly(out, function);
        }
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*> *previous) {
        map<IRKind, set<HashOfQuaternion>*> cur;
        for (int i = 0; i < (int) IRKind::END; i++) {
            cur[(IRKind) i] = new set<HashOfQuaternion>;
        }
        for (Quaternion *i = head; i; i = i->next) {
            i->globalValueNumbering(constValue, copyValue, previous, &cur);
        }
        for (auto & i : successor) {
            for (Quaternion *j = i->head; j && j->getKind() == IRKind::PHI; j = j->next) {
                auto item = ((PhiFunction*)j)->getItem(this);
                if (constValue->count(*item)) {
                    *item = to_string((*constValue)[*item]);
                } else if (copyValue->count(*item)) {
                    *item = (*copyValue)[*item];
                }
            }
        }
        for (auto & i : dominateChildren) {
            i->globalValueNumbering(constValue, copyValue, previous);
        }
        for (auto & i : cur) {
            for (auto & j : *i.second) {
                (*previous)[i.first]->erase(j);
            }
            delete i.second;
        }
    }

    void registerAllocate(map<string, int> *weight, map<int, string> *register2Variable,
                          map<string, int> *variable2Register) {
        set<string> define;
        set<string> neverUseAfter;
        map<string, Quaternion*> lastUse;
        for (Quaternion *i = head; i; i = i->next) {
            auto use = i->getUse();
            for (auto & j : *use) {
                lastUse[*j] = i;
            }
            delete use;
        }
        for (Quaternion *i = head; i; i = i->next) {
            if (i->getKind() != IRKind::PHI) {
                auto use = i->getUse();
                for (auto & j : *use) {
                    if (lastUse[*j] == i && !activenessOut.count(*j) && variable2Register->count(*j)) {
                        register2Variable->erase((*variable2Register)[*j]);
                        neverUseAfter.insert(*j);
                    }
                }
                delete use;
            }
            if (i->getDefine() != nullptr && weight->count(*i->getDefine())) {
                define.insert(*i->getDefine());
                int reg = RegisterAllocate::allocateRegister(weight, register2Variable, i->getDefine());
                if (reg >= 0) {
                    if (register2Variable->count(reg)) {
                        variable2Register->erase((*register2Variable)[reg]);
                    }
                    (*register2Variable)[reg] = *i->getDefine();
                    (*variable2Register)[*i->getDefine()] = reg;
                }
            }
        }
        for (auto & i : dominateChildren) {
            map<int, string> notInSuccessor;
            for (auto j = register2Variable->begin(); j != register2Variable->end();) {
                if (!i->activenessIn.count(j->second)) {
                    notInSuccessor[j->first] = j->second;
                    register2Variable->erase(j++);
                } else {
                    j++;
                }
            }
            i->registerAllocate(weight, register2Variable, variable2Register);
            for (auto & j : notInSuccessor) {
                (*register2Variable)[j.first] = j.second;
            }
        }
        for (auto & i : define) {
            if (variable2Register->count(i)) {
                register2Variable->erase((*variable2Register)[i]);
            }
        }
        for (auto & i : neverUseAfter) {
            if (variable2Register->count(i) && !define.count(i)) {
                (*register2Variable)[(*variable2Register)[i]] = i;
            }
        }
    }
};

void PhiFunction::print(ofstream *out) {
    *out << "phi " << assigned;
    for (auto &i: items) {
        *out << " " << i.first << " " << *i.second->getTag();
    }
    *out << endl;
}
#endif //SYSYCOMPILER_BASICBLOCK_H
