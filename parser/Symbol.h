//
// Created by gyp on 2021/10/15.
//

#ifndef SYSYCOMPILER_SYMBOL_H
#define SYSYCOMPILER_SYMBOL_H
#include <bits/stdc++.h>
#include "../control-flow-graphs/lib.h"
using namespace std;

class LValSymbol {
    bool isConst;
    bool isGlobal;
    int id;
    vector<int> dimension; // upper limit of each dimension
    vector<int> initVal;
    int getOffset(vector<int> *indices) {
        if (indices->empty()) {
            return 0;
        }
        int base = 1, ret = 0;
        for (int i = (int) dimension.size() - 1; i >= indices->size(); i--) {
            base *= dimension[i];
        }
        for (int i = (int) indices->size() - 1; i > 0; i--) {
            ret += (*indices)[i] * base;
            base *= dimension[i];
        }
        return ret + (*indices)[0] * base;
    }
public:
    LValSymbol(bool isConst, bool isGlobal, vector<int>* dimension, vector<int> *init, int id) :
        isConst(isConst), isGlobal(isGlobal), dimension(*dimension), initVal(*init), id(id) {}

    int getDimension() {
        return (int) dimension.size();
    }

    bool getGlobal() {
        return isGlobal;
    }

    int getID() const {
        return id;
    }

    bool getConst() const {
        return isConst;
    }

    int getVal(vector<int> *index) {
        int offset = getOffset(index);
        if (initVal.empty()) {
            return 0;
        } else if (offset >= initVal.size()) {
            cout << "array out of bound" << endl;
            return -1;
        }
        return initVal[offset];
    }

    string* calcOffset(vector<string*> *indices) {
        auto *ret = new string(controlFlowGraphBuilder.tmpVar());
        string base = controlFlowGraphBuilder.tmpVar();
        string tmp = "1";
        controlFlowGraphBuilder.insert(new Assign(&base, &tmp));
        for (int i = (int) dimension.size() - 1; i >= indices->size(); i--) {
            tmp = to_string(dimension[i]);
            controlFlowGraphBuilder.insert(new Multiply(&base, &base, &tmp));
        }
        tmp = "0";
        controlFlowGraphBuilder.insert(new Assign(ret, &tmp));
        for (int i = (int) indices->size() - 1; i >= 0; i--) {
            tmp = controlFlowGraphBuilder.tmpVar();
            controlFlowGraphBuilder.insert(new Multiply(&tmp, (*indices)[i], &base));
            controlFlowGraphBuilder.insert(new Plus(ret, ret, &tmp));
            tmp = to_string(dimension[i]);
            controlFlowGraphBuilder.insert(new Multiply(&base, &base, &tmp));
        }
        return ret;
    }

    void print(ofstream *out) {
        int size = 1;
        for (auto & i : dimension) {
            size *= i;
        }
        if (IR) {
            *out << size << " ";
            for (auto & i : initVal) {
                *out << i << " ";
            }
        } else {
            *out << " : .word ";
            if (initVal.empty()) {
                *out << " 0:" << size << endl;
            } else {
                for (auto & i : initVal) {
                    *out << i << " ";
                }
                *out << endl;
            }
        }
    }
};

class FuncSymbol {
    vector<int> params; // dimension of each param
    string name;
    bool isVoid;
public:
    FuncSymbol(vector<int> *params, string name, bool isVoid) :
    params(*params), name(move(name)), isVoid(isVoid) {}

    ErrorKind matchParams(vector<int> *args) {
        if (args->size() != params.size()) {
            return ErrorKind::PARAMS_NUM_UNMATCHED;
        }
        for (int i = 0; i < params.size(); i++) {
            if (params[i] != (*args)[i]) {
                return ErrorKind::PARAM_TYPE_UNMATCHED;
            }
        }
        return ErrorKind::CORRECT;
    }

    bool getVoid() const {
        return isVoid;
    }
};

class Symbol {
    map<string, FuncSymbol*> funcs;
    map<string, stack<LValSymbol*>*> vars;
    stack<set<string>*> symbolStack;
    map<string, LValSymbol*> globalVal;
    set<pair<string, LValSymbol*>> constVal;
    map<string, string*> constStr;
    int cnt;
public:
    Symbol() : cnt(0) {}

    void clear() {
        for (auto & i : funcs) {
            delete i.second;
        }
        funcs.clear();
        for (auto & i : vars) {
            while(!i.second->empty()) {
                delete i.second->top();
                i.second->pop();
            }
            delete i.second;
        }
        vars.clear();
        while (!symbolStack.empty()) {
            delete symbolStack.top();
            symbolStack.pop();
        }
        globalVal.clear();
        for (auto & i : constVal) {
            delete i.second;
        }
        constVal.clear();
        cnt = 0;
        for (auto & i : constStr) {
            delete i.second;
        }
        constStr.clear();
    }

    bool addFunc(vector<int> *params, string *name, bool isVoid) {
        if (funcs.count(*name)) {
            return false;
        }
        funcs[*name] = new FuncSymbol(params, *name, isVoid);
        return true;
    }

    void addFunc(string *name) {
        vector<int> tmp;
        addFunc(&tmp, name, false);
    }

    int addVar(bool isConst, vector<int>* dimension, string *name, vector<int> *initVal) {
        set<string> *cur = symbolStack.top();
        if (cur->count(*name)) {
            return 0;
        }

        int ret = ++cnt;
        auto lValSymbol = new LValSymbol(isConst, symbolStack.size() == 1, dimension, initVal, ret);
        if (symbolStack.size() == 1) {
            globalVal[generateIRVar(name, ret)] = lValSymbol;
        }
        cur->insert(*name);
        if (vars.count(*name) == 0) {
            vars[*name] = new stack<LValSymbol*>;
        }
        vars[*name]->push(lValSymbol);
        return ret;
    }

    int addVar(bool isConst, vector<int>* dimension, string *name) {
        vector<int> tmp;
        return addVar(isConst, dimension, name, &tmp);
    }

    LValSymbol* gerVar(string *name) {
        if (!vars.count(*name)) {
            return nullptr;
        }
        return vars[*name]->top();
    }

    FuncSymbol* getFunc(string *name) {
        if (!funcs.count(*name)) {
            return nullptr;
        }
        return funcs[*name];
    }

    void startBlock() {
        symbolStack.push(new set<string>);
    }

    void finishBlock() {
        set<string> *cur = symbolStack.top();
        for (auto & i : *cur) {
            if (vars[i]->top()->getConst()) {
                int id = vars[i]->top()->getID();
                constVal.insert(pair<string, LValSymbol*>(generateIRVar(&i, id), vars[i]->top()));
            } else {
                delete vars[i]->top();
            }
            vars[i]->pop();
            if (vars[i]->empty()) {
                delete vars[i];
                vars.erase(i);
            }
        }
        delete cur;
        symbolStack.pop();
    }

    void printGlobalVar(ofstream *out) {
        if (IR) {
            for (auto & i : globalVal) {
                *out << i.first << " ";
                i.second->print(out);
                *out << endl;
            }
            for (auto & i : constVal) {
                *out << i.first << " ";
                i.second->print(out);
                *out << endl;
            }
            for (auto & i : constStr) {
                *out << i.first << " \"" << *i.second << "\"" << endl;
            }
        } else {
            for (auto & i : globalVal) {
                *out << i.first << " ";
                i.second->print(out);
            }
            for (auto & i : constVal) {
                *out << i.first << " ";
                i.second->print(out);
            }
            for (auto & i : constStr) {
                *out << i.first << " : .asciiz \"" << *i.second << "\"" << endl;
            }
        }
    }

    int getLayer() {
        return (int) symbolStack.size();
    }

    void addConstStr(string *label, string *str) {
        constStr[*label] = str;
    }
} symbol;
#endif //SYSYCOMPILER_SYMBOL_H
