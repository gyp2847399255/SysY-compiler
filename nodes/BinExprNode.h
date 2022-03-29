//
// Created by gyp on 2021/10/8.
//

#ifndef SYSYCOMPILER_BINEXPRNODE_H
#define SYSYCOMPILER_BINEXPRNODE_H
#include "PrefixExprNode.h"
using namespace std;

enum class BinExprType {
    OR_EXPR,
    ADD_EXPR,
    AND_EXPR,
    EQ_EXPR,
    REL_EXPR,
    MUL_EXPR,
    NONE
};

map<BinExprType, BinExprType> nextBinExprType = {
        {BinExprType::OR_EXPR, BinExprType::AND_EXPR},
        {BinExprType::AND_EXPR, BinExprType::EQ_EXPR},
        {BinExprType::EQ_EXPR, BinExprType::REL_EXPR},
        {BinExprType::REL_EXPR, BinExprType::ADD_EXPR},
        {BinExprType::ADD_EXPR, BinExprType::MUL_EXPR},
        {BinExprType::MUL_EXPR, BinExprType::NONE}
};

map<BinExprType, BinExprType> lastBinExprType = {
        {BinExprType::OR_EXPR, BinExprType::NONE},
        {BinExprType::AND_EXPR, BinExprType::OR_EXPR},
        {BinExprType::EQ_EXPR, BinExprType::AND_EXPR},
        {BinExprType::REL_EXPR, BinExprType::EQ_EXPR},
        {BinExprType::ADD_EXPR, BinExprType::REL_EXPR},
        {BinExprType::MUL_EXPR, BinExprType::ADD_EXPR}
};

map<BinExprType, string> binExprType2Name = {
        {BinExprType::OR_EXPR, "<LOrExp>"},
        {BinExprType::AND_EXPR, "<LAndExp>"},
        {BinExprType::EQ_EXPR, "<EqExp>"},
        {BinExprType::REL_EXPR, "<RelExp>"},
        {BinExprType::ADD_EXPR, "<AddExp>"},
        {BinExprType::MUL_EXPR, "<MulExp>"}
};

class BinExprNode : public Node {
    string res;
    int getBinPri() {
        if (!binOP2priority.count(children[1]->getKind())) {
            printf("BinOP ERROR\n");
        }
        return binOP2priority[children[1]->getKind()];
    }

    BinExprType getCurType() {
        SyntaxKind kind = children[1]->getKind();
        switch (kind) {
            case SyntaxKind::OR_LOGIC:
                return BinExprType::OR_EXPR;
            case SyntaxKind::AND_LOGIC:
                return BinExprType::AND_EXPR;
            case SyntaxKind::EQ:
            case SyntaxKind::NEQ:
                return BinExprType::EQ_EXPR;
            case SyntaxKind::LE:
            case SyntaxKind::GE:
            case SyntaxKind::LT:
            case SyntaxKind::GT:
                return BinExprType::REL_EXPR;
            case SyntaxKind::PLUS:
            case SyntaxKind::MINUS:
                return BinExprType::ADD_EXPR;
            case SyntaxKind::MULTI:
            case SyntaxKind::DIV:
            case SyntaxKind::MOD:
                return BinExprType::MUL_EXPR;
            default:
                return BinExprType::NONE;
        }
    }

    static void printPrefix(ofstream *out, BinExprType type) {
        if (type == BinExprType::NONE) {
            return;
        }
        *out << "<MulExp>" << endl;
        if (type == BinExprType::MUL_EXPR) {
            return;
        }
        *out << "<AddExp>" << endl;
        if (type == BinExprType::ADD_EXPR) {
            return;
        }
        *out << "<RelExp>" << endl;
        if (type == BinExprType::REL_EXPR) {
            return;
        }
        *out << "<EqExp>" << endl;
        if (type == BinExprType::EQ_EXPR) {
            return;
        }
        *out << "<LAndExp>" << endl;
        if (type == BinExprType::AND_EXPR) {
            return;
        }
        *out << "<LOrExp>" << endl;
    }

    void getRes(string *x, string *y, SyntaxKind kind) {
        switch (kind) {
            case SyntaxKind::NEQ:
                controlFlowGraphBuilder.insert(new NotEqual(&res, x, y));
                return;
            case SyntaxKind::EQ:
                controlFlowGraphBuilder.insert(new Equal(&res, x, y));
                return;
            case SyntaxKind::LT:
                controlFlowGraphBuilder.insert(new LessThan(&res, x, y));
                return;
            case SyntaxKind::GT:
                controlFlowGraphBuilder.insert(new GreaterThan(&res, x, y));
                return;
            case SyntaxKind::LE:
                controlFlowGraphBuilder.insert(new LessOrEqual(&res, x, y));
                return;
            case SyntaxKind::GE:
                controlFlowGraphBuilder.insert(new GreaterOrEqual(&res, x, y));
                return;
            case SyntaxKind::PLUS:
                controlFlowGraphBuilder.insert(new Plus(&res, x, y));
                return;
            case SyntaxKind::MINUS:
                controlFlowGraphBuilder.insert(new Minus(&res, x, y));
                return;
            case SyntaxKind::MULTI:
                controlFlowGraphBuilder.insert(new Multiply(&res, x, y));
                return;
            case SyntaxKind::DIV:
                controlFlowGraphBuilder.insert(new Divide(&res, x, y));
                return;
            case SyntaxKind::MOD:
                controlFlowGraphBuilder.insert(new Mod(&res, x, y));
                return;
            default:
                printf("Unrecognizable Binary OP\n");
        }
    }

    static int calcVal(int x, int y, SyntaxKind kind) {
        switch (kind) {
            case SyntaxKind::OR_LOGIC:
                return x || y;
            case SyntaxKind::AND_LOGIC:
                return x && y;
            case SyntaxKind::NEQ:
                return x != y;
            case SyntaxKind::EQ:
                return x == y;
            case SyntaxKind::LT:
                return x < y;
            case SyntaxKind::GT:
                return x > y;
            case SyntaxKind::LE:
                return x <= y;
            case SyntaxKind::GE:
                return x >= y;
            case SyntaxKind::PLUS:
                return x + y;
            case SyntaxKind::MINUS:
                return x - y;
            case SyntaxKind::MULTI:
                return x * y;
            case SyntaxKind::DIV:
                return x / y;
            case SyntaxKind::MOD:
                return x % y;
            default:
                printf("Unrecognizable Binary OP\n");
                return 0;
        }
    }

    void orIR(BuildIRCtx *ctx) {
        auto ret0 = new BuildIRRet();
        children[0]->buildIR(ctx, ret0);
        BasicBlock *cfg1, *cfg2;
        bool flag = false;
        if (exprResIsNumber(ret0->res)) {
            if (stoi(*ret0->res)) {
                this->res = "1";
                delete ret0;
                return;
            }
            flag = true;
        } else {
            cfg1 = controlFlowGraphBuilder.newBasicBlock();
            this->res = controlFlowGraphBuilder.tmpVar();
            controlFlowGraphBuilder.insert(new Assign(&res, ret0->res));
            controlFlowGraphBuilder.insert(new JumpWhenTrue(cfg1, &res));
            cfg2 = controlFlowGraphBuilder.newBasicBlock();
            controlFlowGraphBuilder.changeCur(cfg2);
        }
        delete ret0;
        ret0 = new BuildIRRet;
        children[2]->buildIR(ctx, ret0);
        if (flag) {
            if (exprResIsNumber(ret0->res)) {
                if (stoi(*ret0->res)) {
                    this->res = "1";
                } else {
                    this->res = "0";
                }
                delete ret0;
                return;
            } else {
                this->res = controlFlowGraphBuilder.tmpVar();
            }
        }
        controlFlowGraphBuilder.insert(new Assign(&res, ret0->res));
        if (!flag) {
            controlFlowGraphBuilder.changeCur(cfg1);
        }
    }

    void andIR(BuildIRCtx *ctx) {
        auto ret0 = new BuildIRRet();
        children[0]->buildIR(ctx, ret0);
        BasicBlock *cfg1, *cfg2;
        bool flag = false;
        if (exprResIsNumber(ret0->res)) {
            if (!stoi(*ret0->res)) {
                this->res = "0";
                delete ret0;
                return;
            }
            flag = true;
        } else {
            cfg1 = controlFlowGraphBuilder.newBasicBlock();
            this->res = controlFlowGraphBuilder.tmpVar();
            controlFlowGraphBuilder.insert(new Assign(&res, ret0->res));
            controlFlowGraphBuilder.insert(new JumpWhenFalse(cfg1, &res));
            cfg2 = controlFlowGraphBuilder.newBasicBlock();
            controlFlowGraphBuilder.changeCur(cfg2);
        }
        delete ret0;
        ret0 = new BuildIRRet;
        children[2]->buildIR(ctx, ret0);
        if (flag) {
            if (exprResIsNumber(ret0->res)) {
                if (!stoi(*ret0->res)) {
                    this->res = "0";
                } else {
                    this->res = "1";
                }
                delete ret0;
                return;
            } else {
                this->res = controlFlowGraphBuilder.tmpVar();
            }
        }
        controlFlowGraphBuilder.insert(new Assign(&res, ret0->res));
        if (!flag) {
            controlFlowGraphBuilder.changeCur(cfg1);
        }
    }
public:
    void print(ofstream *out, BinExprType type) {
        BinExprType curType = getCurType();
        if (children[0]->getKind() == SyntaxKind::PREFIX_EXPR) {
            ((PrefixExprNode*)children[0])->print(out, PrefixExprType::PREFIX_EXPR);
            printPrefix(out, curType);
        } else if (children[0]->getKind() == SyntaxKind::BIN_EXPR) {
            if (getBinPri() == ((BinExprNode*)children[0])->getBinPri()) {
                ((BinExprNode*)children[0])->print(out, curType);
            } else {
                ((BinExprNode*)children[0])->print(out,  nextBinExprType[curType]);
                *out << binExprType2Name[curType] << endl;
            }
        }
        children[1]->print(out);
        if (children[2]->getKind() == SyntaxKind::BIN_EXPR) {
            ((BinExprNode*)children[2])->print(out, nextBinExprType[curType]);
        } else if (children[2]->getKind() == SyntaxKind::PREFIX_EXPR) {
            ((PrefixExprNode*)children[2])->print(out, PrefixExprType::PREFIX_EXPR);
            printPrefix(out, nextBinExprType[curType]);
        }
        while (curType != type) {
            *out << binExprType2Name[curType] << endl;
            curType = lastBinExprType[curType];
        }
        *out << binExprType2Name[type] << endl;
    }

    void checkError(ErrorCheckCtx *ctx, ErrorCheckRet *ret) override {
        if (ctx->isLVal) {
            errorList.emplace_back(ErrorKind::UNDEFINED_ERROR, this->finishLine);
            return;
        }
        for (auto & i : children) {
            i->checkError(ctx, ret);
        }
    }

    void buildIR(BuildIRCtx *ctx, BuildIRRet *ret) override {
        ret->res = &this->res;
        if (children[1]->getKind() == SyntaxKind::OR_LOGIC) {
            orIR(ctx);
        } else if (children[1]->getKind() == SyntaxKind::AND_LOGIC) {
            andIR(ctx);
        } else {
            BuildIRRet *ret0[3];
            for (auto & i : ret0) {
                i = new BuildIRRet();
            }
            for (int i = 0; i < children.size(); i++) {
                children[i]->buildIR(ctx, ret0[i]);
            }
            if (exprResIsNumber(ret0[0]->res) && exprResIsNumber(ret0[2]->res)) {
                int v1 = stoi(*ret0[0]->res);
                int v2 = stoi(*ret0[2]->res);
                this->res = to_string(calcVal(v1, v2, children[1]->getKind()));
            } else {
                this->res = controlFlowGraphBuilder.tmpVar();
                getRes(ret0[0]->res, ret0[2]->res, children[1]->getKind());
            }
            for (auto & i : ret0) {
                delete i;
            }
        }
    }
};
#endif //SYSYCOMPILER_BINEXPRNODE_H
