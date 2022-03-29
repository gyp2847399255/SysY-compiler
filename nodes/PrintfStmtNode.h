//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_PRINTFSTMTNODE_H
#define SYSYCOMPILER_PRINTFSTMTNODE_H
#include "Node.h"
using namespace std;

class PrintfStmtNode : public Node {
public:
    void print(ofstream *out) override {
        for (auto & i : children) {
            i->print(out);
        }
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        int cnt = 0;
        int line;
        string *formatString;
        for (auto & i : children) {
            i->checkError(ctx, ret);
            if (i->getKind() == SyntaxKind::EXPR) {
                cnt++;
            } else if (i->getKind() == SyntaxKind::STRING) {
                formatString = ((TokenNode*)i)->getContent();
                line = i->getFinishLine();
            }
        }
        bool formatStringError = false;
        for (int i = 1; i < formatString->size() - 1; i++) {
            char c = (*formatString)[i];
            char last = (*formatString)[i - 1];
            if (c != 32 && c != 33 && c != '%' && (c < 40 || c > 126)) {
                formatStringError = true;
            } else if (last == '\\' && c != 'n') {
                formatStringError = true;
            } else if (last == '%' && c != 'd') {
                formatStringError = true;
            }
            if (last == '%' && c == 'd') {
                cnt--;
            }
        }
        if (cnt != 0) {
            errorList.emplace_back(ErrorKind::FORMAT_CHAR_UNMATCHED, this->startLine);
        }
        if (formatStringError) {
            errorList.emplace_back(ErrorKind::INVALID_CHARACTER, line);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        vector<string*> integers;
        string *formatString;
        for (auto & i : children) {
            if (i->getKind() == SyntaxKind::EXPR) {
                i->buildIR(ctx, ret);
                integers.push_back(ret->res);
            } else if (i->getKind() == SyntaxKind::STRING) {
                formatString = ((TokenNode*)i)->getContent();
            }
        }
        string now;
        for (int i = 1, j = 0; i < formatString->size() - 1; i++) {
            char c = (*formatString)[i];
            if (c == '%' && (*formatString)[i + 1] == 'd') {
                string label = generateStr();
                symbol.addConstStr(&label, new string(now));
                if (!now.empty()) {
                    now.clear();
                    controlFlowGraphBuilder.insert(new PrintString(&label));
                }
                controlFlowGraphBuilder.insert(new PrintInteger(integers[j]));
                j++;
                i++;
            } else {
                now += c;
            }
        }
        string label = generateStr();
        symbol.addConstStr(&label, new string(now));
        if (!now.empty()) {
            controlFlowGraphBuilder.insert(new PrintString(&label));
        }
    }
};
#endif //SYSYCOMPILER_PRINTFSTMTNODE_H
