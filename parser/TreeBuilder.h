//
// Created by gyp on 2021/9/26.
//

#ifndef SYSYCOMPILER_TREEBUILDER_H
#define SYSYCOMPILER_TREEBUILDER_H
#include "../lexer/lib.h"
#include "../nodes/lib.h"
#include "ErrorKind.h"

class TreeBuilder {
    typedef pair<SyntaxKind, int> Parent;
    stack<Parent> parent;
    stack<Node*> children;
public:
    void startNode(SyntaxKind kind) {
        parent.push(Parent(kind, children.size()));
    }
    void finishNode(Node *node) {
        Parent tmp = parent.top();
        parent.pop();
        while (children.size() > tmp.second) {
            node->addChild(children.top());
            children.pop();
        }
        node->setNodeElement(tmp.first);
        children.push(node);
    }

    // make left brother left son
    void startNodeAt(int point, SyntaxKind kind) {
        parent.push(Parent(kind, point));
    }

    void terminalSymbol(SyntaxKind kind, string *content, int line) {
        auto *node = new TokenNode(kind, content, line);
        children.push(node);
    }

    void error(ErrorKind errorKind, int line) {
        Node* node = new ErrorNode(errorKind, line);
        node->setNodeElement(SyntaxKind::ERROR, line);
        if (errorKind != ErrorKind::UNDEFINED_ERROR) {
            children.push(node);
            return;
        }
        Parent tmp = parent.top();
        parent.pop();
        while (children.size() > tmp.second) {
            node->addChild(children.top());
            children.pop();
        }
        children.push(node);
    }

    int checkPoint() {
        return children.size();
    }

    Node *root() {
        if (parent.size() == 0) {
            return children.top();
        } else {
            printf("Parent is not empty, get root FAILED\n");
            return children.top();
        }
    }
};
#endif //SYSYCOMPILER_TREEBUILDER_H
