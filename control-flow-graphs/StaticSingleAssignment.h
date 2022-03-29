//
// Created by gyp on 2021/11/18.
//

#ifndef SYSYCOMPILER_STATICSINGLEASSIGNMENT_H
#define SYSYCOMPILER_STATICSINGLEASSIGNMENT_H
class StaticSingleAssignment {
    BasicBlock **head;
    BasicBlock **tail;
    map<string, set<BasicBlock*>*> defineBlocks;
    void getEdges() {
        for (BasicBlock *i = *head; i; i = i->next) {
            if (i->accessToNextBlock()) {
                i->addSuccessor(i->next);
                i->next->addPredecessor(i);
            }
            auto jumpBlock = i->getJumpBlock();
            if (jumpBlock != nullptr) {
                i->addSuccessor(jumpBlock);
                jumpBlock->addPredecessor(i);
            }
        }
    }

    void updateDominator(BasicBlock *basicBlock) {
        if (basicBlock == *head) {
            for (BasicBlock *i = *head; i; i = i->next) {
                i->addDominator(basicBlock);
            }
            return;
        }
        set<BasicBlock*> vis;
        vis.insert(*head);
        queue<BasicBlock*> q;
        q.push(*head);
        while (!q.empty()) {
            auto tmp = q.front();
            q.pop();
            for (auto & j : *tmp->getSuccessor()) {
                if (j == basicBlock) {
                    continue;
                }
                if (!vis.count(j)) {
                    vis.insert(j);
                    q.push(j);
                }
            }
        }
        for (BasicBlock *i = *head; i; i = i->next) {
            if (!vis.count(i)) {
                i->addDominator(basicBlock);
            }
        }
    }

    void erase(BasicBlock *basicBlock) {
        if (basicBlock == *head) {
            if (basicBlock == *tail) {
                *head = *tail = nullptr;
                return;
            }
            *head = basicBlock->next;
        } else {
            basicBlock->last->next = basicBlock->next;
        }
        if (basicBlock == *tail) {
            *tail = basicBlock->last;
        } else {
            basicBlock->next->last = basicBlock->last;
        }
    }

    void dominator() {
        set<BasicBlock*> vis;
        queue<BasicBlock*> q;
        q.push(*head);
        while (!q.empty()) {
            BasicBlock *tmp = q.front();
            q.pop();
            for (auto & i : *tmp->getSuccessor()) {
                if (!vis.count(i)) {
                    vis.insert(i);
                    q.push(i);
                }
            }
        }
        vis.insert(*head);
        for (BasicBlock *i = *head; i; i = i->next) {
            if (!vis.count(i)) {
                erase(i);
            } else {
                for (auto it = i->getSuccessor()->begin(); it != i->getSuccessor()->end();) {
                    if (!vis.count(*it)) {
                        i->getSuccessor()->erase(it++);
                    } else {
                        it++;
                    }
                }
                for (auto it = i->getPredecessor()->begin(); it != i->getPredecessor()->end();) {
                    if (!vis.count(*it)) {
                        i->getPredecessor()->erase(it++);
                    } else {
                        it++;
                    }
                }
            }
        }
        for (BasicBlock *i = *head; i; i = i->next) {
            updateDominator(i);
        }
        for (BasicBlock *i = *head; i; i = i->next) {
            i->calcImmediateDominator();
        }
        int dfn = 0;
        (*head)->dfsDominanceTree(&dfn);
    }

    void getDefine() {
        for (BasicBlock *i = *head; i; i = i->next) {
            for (Quaternion *j = i->getHead(); j; j = j->next) {
                string *define = j->getDefine();
                if (define == nullptr) {
                    continue;
                }
                if (!defineBlocks.count(*define)) {
                    defineBlocks[*define] = new set<BasicBlock*>;
                }
                defineBlocks[*define]->insert(i);
            }
        }
        for (auto & i : defineBlocks) {
            if (i.second->size() > 1) {
                string tmp = "19260817";
                (*head)->insertToFront(new Assign(&i.first, &tmp));
            }
        }
    }

    void dominanceFrontier() {
        for (BasicBlock *i = *head; i; i = i->next) {
            for (BasicBlock *j = *head; j; j = j->next) {
                for (auto & k : *j->getPredecessor()) {
                    if (i->dominates(k) && (!i->dominates(j) || i == j)) {
                        i->addDominatorFrontier(j);
                    }
                }
            }
        }
    }

    void addPhi() {
        for (auto & i : defineBlocks) {
            if (i.second->size() <= 1) {
                continue;
            }
            set<BasicBlock*> phiNode;
            queue<BasicBlock*> q;
            for (auto & j : *i.second) {
                q.push(j);
            }
            while (!q.empty()) {
                BasicBlock *tmp = q.front();
                q.pop();
                for (auto j : *tmp->getDominatorFrontier()) {
                    if (!phiNode.count(j)) {
                        phiNode.insert(j);
                        q.push(j);
                    }
                }
            }
            for (auto & j : phiNode) {
                j->addPhi(&i.first);
            }
        }
        for (BasicBlock *i = *head; i; i = i->next) {
            i->insertPhiToList();
        }
    }

    map<string, int> defineNameNumber;
    map<string, int> useNameNumber;
    set<BasicBlock*> renamedBlock;
    void rename(BasicBlock *basicBlock) {
        renamedBlock.insert(basicBlock);
        map<string, int> newDefinedName;
        for (Quaternion *i = basicBlock->getHead(); i; i = i->next) {
            auto used = i->getUse();
            for (auto & j : *used) {
                if (useNameNumber.count(*j)) {
                    *j += "_" + to_string(useNameNumber[*j]);
                }
            }
            delete used;
            string *assigned = i->getDefine();
            if (assigned != nullptr) {
                if (defineNameNumber.count(*assigned)) {
                    defineNameNumber[*assigned]++;
                } else {
                    defineNameNumber[*assigned] = 1;
                }
                if (!newDefinedName.count(*assigned)) {
                    newDefinedName[*assigned] = useNameNumber.count(*assigned) ? useNameNumber[*assigned] : 0;
                }
                useNameNumber[*assigned] = defineNameNumber[*assigned];
                *assigned += "_" + to_string(defineNameNumber[*assigned]);
            }
        }
        for (auto & i : *basicBlock->getSuccessor()) {
            for (Quaternion *j = i->getHead(); j->getKind() == IRKind::PHI; j = j->next) {
                string name = *((PhiFunction*)j)->getOriginal();
                name += "_" + to_string(useNameNumber[name]);
                ((PhiFunction*)j)->addItem(&name, basicBlock);
            }
        }
        for (auto & i : *basicBlock->getSuccessor()) {
            if (!renamedBlock.count(i)) {
                rename(i);
            }
        }
        for (auto & i : newDefinedName) {
            if (i.second == 0) {
                useNameNumber.erase(i.first);
            } else {
                useNameNumber[i.first] = i.second;
            }
        }
    }
public:
    StaticSingleAssignment(BasicBlock **head, BasicBlock **tail) : head(head), tail(tail) {}

    void execute() {
        getEdges();
        dominator();
        dominanceFrontier();
        getDefine();
        addPhi();
        rename(*head);
    }
};
#endif //SYSYCOMPILER_STATICSINGLEASSIGNMENT_H
