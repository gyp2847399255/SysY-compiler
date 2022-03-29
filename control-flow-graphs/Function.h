//
// Created by gyp on 2021/11/13.
//

#ifndef SYSYCOMPILER_FUNCTION_H
#define SYSYCOMPILER_FUNCTION_H
#include "BasicBlock.h"
#include "StaticSingleAssignment.h"

class Function {
    map<string, int> variable2Register;
    map<string, int> variableInMemory;
    map<string, int> arrayInMemory;
    map<string, int> variableWeight;
    BasicBlock *head;
    BasicBlock *tail;
    int memory;
    string tag;

    map<string, Quaternion*> defPlace;
    map<string, set<Quaternion*>*> usePlace;
    void insertDef(Quaternion *quaternion) {
        auto name = quaternion->getDefine();
        if (name != nullptr) {
            defPlace[*name] = quaternion;
        }
    }

    void insertUse(Quaternion *quaternion) {
        auto use = quaternion->getUse();
        for (auto & j : *use) {
            if (!usePlace.count(*j)) {
                usePlace[*j] = new set<Quaternion*>();
            }
            usePlace[*j]->insert(quaternion);
        }
        delete use;
    }

    void getDefineUse() {
        defPlace.clear();
        usePlace.clear();
        set<BasicBlock*> vis;
        queue<BasicBlock*> q;
        q.push(head);
        while (!q.empty()) {
            BasicBlock *tmp = q.front();
            q.pop();
            for (Quaternion *i = tmp->getHead(); i; i = i->next) {
                insertDef(i);
                insertUse(i);
            }
            for (auto & i : *tmp->getSuccessor()) {
                if (!vis.count(i)) {
                    q.push(i);
                    vis.insert(i);
                }
            }
        }
    }

    static void eliminateQuaternion(Quaternion *quaternion) {
        quaternion->belong->eliminateQuaternion(quaternion);
    }

    void eliminateDefine() {
        queue<const string*> q;
        for (auto & i : defPlace) {
            if (!usePlace.count(i.first) || usePlace[i.first]->empty()) {
                q.push(&i.first);
            }
        }
        while (!q.empty()) {
            const string *tmp = q.front();
            auto quaternion = defPlace[*tmp];
            q.pop();
            auto use = quaternion->getUse();
            for (auto & i : *use) {
                if (!defPlace.count(*i)) {
                    continue;
                }
                usePlace[*i]->erase(quaternion);
                if (usePlace[*i]->empty()) {
                    q.push(defPlace[*i]->getDefine());
                }
            }
            eliminateQuaternion(quaternion);
            delete use;
        }
    }

    void globalValueNumbering() {
        map<string, int> constValue;
        map<string, string> copyValue;
        map<IRKind, map<HashOfQuaternion, string>*> previous;
        for (int i = 0; i < (int) IRKind::END; i = i + 1) {
            previous[(IRKind) i] = new map<HashOfQuaternion, string>();
        }
        head->globalValueNumbering(&constValue, &copyValue, &previous);
        for (auto & i : previous) {
            delete i.second;
        }
    }

    void getOutVariable() {
        for (BasicBlock *i = head; i; i = i->next) {
            for (Quaternion *j = i->getHead(); j; j = j->next) {
                i->insertDefine(j);
                i->insertUse(j);
            }
        }
        for (BasicBlock *i = head; i; i = i->next) {
            i->getOutVariable();
        }
    }

    void getVariableWeight(BasicBlock *basicBlock) {
        for (BasicBlock *i = basicBlock; i; i = i->next) {
            for (Quaternion *j = i->getHead(); j; j = j->next) {
                string *assigned = j->getDefine();
                if (j->getKind() == IRKind::CREATE_POINTER) {
                    assigned = nullptr;
                }
                if (assigned != nullptr) {
                    variableWeight[*assigned] = 1;
                }
            }
        }
    }

    void rebuildPhi(BasicBlock *from, BasicBlock *to, vector<pair<string*, string*>> *phi) {
        BasicBlock *use = from;
        bool needJump = false;
        if (from->getSuccessor()->size() == 2) {
            use = new BasicBlock(from, to);
            if (from->getJumpBlock() == to) {
                from->setJumpBlock(use);
                needJump = true;
                tail->next = use;
                use->last = tail;
                tail = use;
            } else {
                use->next = from->next;
                use->last = from;
                from->next->last = use;
                from->next = use;
            }
        } else if (from->getTail() != nullptr && from->getTail()->getKind() == IRKind::JUMP) {
            eliminateQuaternion(from->getTail());
            needJump = true;
        }
        vector<int> edges[phi->size()];
        int inDegree[phi->size()];
        bool vis[phi->size()];
        memset(vis, 0, sizeof vis);
        memset(inDegree, 0, sizeof inDegree);
        for (int i = 0; i < phi->size(); i++) {
            for (int j = 0; j < phi->size(); j++) {
                if (i == j) {
                    continue;
                }
                if (*(*phi)[i].second == *(*phi)[j].first) {
                    edges[i].push_back(j);
                    inDegree[j]++;
                }
            }
        }
        queue<int> q;
        for (int i = 0; i < phi->size(); i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
        while (true) {
            string intermediate = "$28";
            while (!q.empty()) {
                int tmp = q.front();
                q.pop();
                use->insert(new Assign((*phi)[tmp].first, (*phi)[tmp].second));
                vis[tmp] = true;
                for (auto & i : edges[tmp]) {
                    inDegree[i]--;
                    if (inDegree[i] == 0) {
                        q.push(i);
                    }
                }
            }
            bool flag = true;
            for (int i = 0; i < phi->size(); i++) {
                if (!vis[i]) {
                    flag = false;
                    use->insert(new Assign(&intermediate, (*phi)[i].second));
                    (*phi)[i].second = &intermediate;
                    for (auto & j : edges[i]) {
                        inDegree[j]--;
                        if (inDegree[j] == 0) {
                            q.push(j);
                        }
                    }
                    break;
                }
            }
            if (flag) {
                break;
            }
        }
        if (needJump) {
            use->insert(new Jump(to));
        }
    }

    void decomposePhi() {
        for (BasicBlock *i = head->next; i; i = i->next) {
            if (i->getHead() != nullptr && i->getHead()->getKind() == IRKind::PHI) {
                for (auto & j : *i->getPredecessor()) {
                    vector<pair<string*, string*>> tmp;
                    for (Quaternion *k = i->getHead(); k != nullptr && k->getKind() == IRKind::PHI; k = k->next) {
                        if (*k->getDefine() != *((PhiFunction*)k)->getItem(j)) {
                            tmp.emplace_back(k->getDefine(), ((PhiFunction*)k)->getItem(j));
                        }
                    }
                    if (!tmp.empty()) {
                        rebuildPhi(j, i, &tmp);
                    }
                }
                for (Quaternion *j = i->getHead(); j != nullptr && j->getKind() == IRKind::PHI; j = j->next) {
                    eliminateQuaternion(j);
                }
            }
        }
    }
    map<int, int> registerInMemory;
    unsigned int paramNumber;
public:
    explicit Function(string *tag, unsigned int paramNumber) : tag(*tag),
    head(nullptr), tail(nullptr), memory(0), paramNumber(paramNumber) {}

    void addBasicBlock(BasicBlock *basicBlock) {
        if (head == nullptr) {
            head = tail = basicBlock;
        } else {
            tail->next = basicBlock;
            basicBlock->last = tail;
            tail = basicBlock;
        }
    }

    void simplify() {
        for (BasicBlock *i = head; i; i = i->next) {
            i->simplify();
            if (i->getJumpBlock() == nullptr || i->getJumpBlock()->getHead() != nullptr) {
                continue;
            }
            while (i->getJumpBlock()->getHead() == nullptr) {
                i->setJumpBlock(i->getJumpBlock()->next);
            }
        }
        for (BasicBlock *i = head->next; i; i = i->next) {
            if (i->getHead() == nullptr) {
                i->last->next = i->next;
                i->next->last = i->last;
            }
        }
    }

    void print(ofstream *out) {
        *out << tag << " :" << endl;
        for (BasicBlock *i = head; i; i = i->next) {
            i->print(out);
        }
    }

    void staticSingleAssignment() {
        StaticSingleAssignment staticSingleAssignment(&head, &tail);
        staticSingleAssignment.execute();
    }

    void optimize() {
        getDefineUse();
        eliminateDefine();
        globalValueNumbering();
        getDefineUse();
        eliminateDefine();
        if (IR) {
            decomposePhi();
        }
    }

    void registerAllocate() {
        getOutVariable();
        getVariableWeight(head);
        map<int, string> register2Variable;
        if (OPT) {
            head->registerAllocate(&variableWeight, &register2Variable,
                                   &variable2Register);
        }
        for (BasicBlock *i = head; i; i = i->next) {
            for (Quaternion *j = i->getHead(); j; j = j->next) {
                auto define = j->getDefine();
                if (define && !variable2Register.count(*define)) {
                    if (j->getKind() == IRKind::CREATE_POINTER) {
                        arrayInMemory[*define] = memory;
                        memory += ((CreatePointer*)j)->getSize() * 4;
                    } else {
                        variableInMemory[*define] = memory;
                        memory += 4;
                    }
                }
            }
        }
        for (auto & i : variable2Register) {
            if (!registerInMemory.count(i.second)) {
                registerInMemory[i.second] = memory;
                memory += 4;
            }
        }
        registerInMemory[31] = memory;
        memory += 4;
    }

    void functionReturn(ofstream *out) {
        for (auto & i : registerInMemory) {
            *out << "lw $" << i.first << ", " << i.second << "($sp)" << endl;
        }
        *out << "addi $sp, $sp, " << memory << endl;
        *out << "jr $31" << endl;
    }

    void assembly(ofstream *out) {
        for (BasicBlock *i = head; i; i = i->next) {
            i->rename(&variable2Register, &variableInMemory, &arrayInMemory);
        }
        decomposePhi();
        *out << tag << ":" << endl;
        memory += paramNumber > 4 ? paramNumber * 4 - 16 : 0;
        *out << "subi $sp, $sp, " << memory << endl;
        for (auto & i : registerInMemory) {
            *out << "sw $" << i.first << ", " << i.second << "($sp)" << endl;
        }
        for (BasicBlock *i = head; i; i = i->next) {
            i->assembly(out, this);
        }
    }

    int getMemory() const {
        return memory;
    }
};

void LoadParam::assembly(ofstream *out, Function *function) {
    if (num < 4) {
        if (integer[0] == '$') {
            *out << "move " << integer << ", $a" << num << endl;
        } else {
            *out << "sw $a" << num << ", " << integer.substr(2) << "($sp)" << endl;
        }
    } else {
        if (integer[0] == '$') {
            *out << "lw " << integer << ", " << function->getMemory() - (num - 3) * 4 << "($sp)" << endl;
        } else {
            *out << "lw $27, " << function->getMemory() - (num - 3) * 4 << "($sp)" << endl;
            *out << "sw $27, " << integer.substr(2) << "($sp)" << endl;
        }
    }
}

void Return::assembly(ofstream *out, Function *function) {
    if (integer == "-") {
        function->functionReturn(out);
        return;
    }
    if (integer[0] == '$') {
        *out << "move $v0, " << integer << endl;
    } else if (integer[0] == 's') {
        *out << "lw $v0, " << integer.substr(2) << "($sp)" << endl;
    } else {
        *out << "li $v0, " << integer << endl;
    }
    function->functionReturn(out);
}
#endif //SYSYCOMPILER_FUNCTION_H
