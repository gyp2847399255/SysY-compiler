//
// Created by gyp on 2021/11/14.
//

#ifndef SYSYCOMPILER_PHIFUNCTION_H
#define SYSYCOMPILER_PHIFUNCTION_H

class PhiFunction : public Quaternion {
    typedef pair<string, BasicBlock*> PhiItem;
    vector<PhiItem> items;
    string assigned;
    string original;
public:
    explicit PhiFunction(const string *assigned) : Quaternion(IRKind::PHI),
    assigned(*assigned), original(*assigned) {}

    string* getOriginal() {
        return &original;
    }

    string* getDefine() override {
        return &assigned;
    }

    set<string*>* getUse() override {
        auto ret = new set<string*>;
        for (auto & i : items) {
            ret->insert(&i.first);
        }
        return ret;
    }

    void print(ofstream *out) override;

    void addItem(string *name, BasicBlock *basicBlock) {
        items.emplace_back(*name, basicBlock);
    }

    string* getItem(BasicBlock *basicBlock) {
        for (auto & i : items) {
            if (basicBlock == i.second) {
                return &i.first;
            }
        }
        return nullptr;
    }

    void globalValueNumbering(map<string, int> *constValue, map<string, string> *copyValue,
                              map<IRKind, map<HashOfQuaternion, string>*>* previous,
                              map<IRKind, set<HashOfQuaternion>*>* cur) override {
        for (auto & i : items) {
            if (constValue->count(i.first)) {
                i.first = to_string((*constValue)[i.first]);
            } else if (copyValue->count(i.first)) {
                i.first = (*copyValue)[i.first];
            }
        }
    }

    void assembly(ofstream *out, Function*) override {
        *out << "ERROR, use phi to assembly" << endl;
    }
};
#endif //SYSYCOMPILER_PHIFUNCTION_H
