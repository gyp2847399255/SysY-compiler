//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_ERRORNODE_H
#define SYSYCOMPILER_ERRORNODE_H
#include "Node.h"
using namespace std;

class ErrorNode : public Node {
    ErrorKind errorKind;
    int line;
public:
    ErrorNode(ErrorKind errorKind, int line) : errorKind(errorKind), line(line) {}
    void print(ofstream *out) override {
        *out << "<Error>" << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        errorList.emplace_back(errorKind, line);
    }
};
#endif //SYSYCOMPILER_ERRORNODE_H
