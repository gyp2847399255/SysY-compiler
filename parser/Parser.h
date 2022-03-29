//
// Created by gyp on 2021/9/26.
//

#ifndef SYSYCOMPILER_PARSER_H
#define SYSYCOMPILER_PARSER_H
#include "../lexer/lib.h"
#include "../nodes/lib.h"
#include "TreeBuilder.h"
#include "ErrorKind.h"

#define KIND (this->tokens->first()->kind)
#define CHECK_TOKEN(x,y) \
if (tokens->first()->kind != x) { \
error(y); \
if (y==ErrorKind::UNDEFINED_ERROR) \
return; \
} else { \
terminalSymbol(); \
} \
true

class Parser {
private:
    TokenSource *tokens;
    TreeBuilder *builder;

    void error(ErrorKind errorKind) {
        builder->error(errorKind, tokens->lastLine());
    }

    void compileUnit() {
        builder->startNode(SyntaxKind::COMP_UNIT);
        SyntaxKind kind = tokens->first()->kind;
        while (kind != SyntaxKind::EOF_CHAR) {
            if (kind == SyntaxKind::VOID_KW) {
                funcDef();
            } else if (kind == SyntaxKind::CONST_KW) {
                varDecl();
            } else if (kind == SyntaxKind::INT_KW) {
                if (tokens->nthToken(2)->kind == SyntaxKind::L_PAREN) {
                    funcDef();
                } else {
                    varDecl();
                }
            } else {
                error(ErrorKind::UNDEFINED_ERROR);
                return;
            }
            kind = tokens->first()->kind;
        }
        builder->finishNode(new CompileUnitNode());
    }

    void funcType() {
        builder->startNode(SyntaxKind::FUNC_TYPE);
        SyntaxKind kind = tokens->first()->kind;
        if (kind != SyntaxKind::VOID_KW && kind != SyntaxKind::INT_KW) {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        terminalSymbol();
        builder->finishNode(new FuncTypeNode());
    }

    void funcDef() {
        builder->startNode(SyntaxKind::FUNC_DEF);
        funcType();
        SyntaxKind kind = tokens->first()->kind;
        if (kind != SyntaxKind::IDENT && kind != SyntaxKind::MAIN_KW) {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        terminalSymbol();
        CHECK_TOKEN(SyntaxKind::L_PAREN, ErrorKind::UNDEFINED_ERROR);
        while (tokens->first()->kind == SyntaxKind::INT_KW) {
            funcFormalParams();
        }
        CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        block();
        builder->finishNode(new FuncDefNode());
    }

    void funcFormalParams() {
        builder->startNode(SyntaxKind::FUNC_PARAMS);
        funcFormalParam();
        while (tokens->first()->kind == SyntaxKind::COMMA) {
            terminalSymbol();
            funcFormalParam();
        }
        builder->finishNode(new FuncParamsNode());
    }

    void funcFormalParam() {
        builder->startNode(SyntaxKind::FUNC_PARAM);
        CHECK_TOKEN(SyntaxKind::INT_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::IDENT, ErrorKind::UNDEFINED_ERROR);
        if (tokens->first()->kind == SyntaxKind::L_BRACKET) {
            terminalSymbol();
            CHECK_TOKEN(SyntaxKind::R_BRACKET, ErrorKind::R_BRACKET_LACKED);
            while (tokens->first()->kind == SyntaxKind::L_BRACKET) {
                terminalSymbol();
                expr();
                CHECK_TOKEN(SyntaxKind::R_BRACKET, ErrorKind::R_BRACKET_LACKED);
            }
        }
        builder->finishNode(new FuncParamNode());
    }

    void block() {
        builder->startNode(SyntaxKind::BLOCK);
        CHECK_TOKEN(SyntaxKind::L_CURLY, ErrorKind::UNDEFINED_ERROR);
        while (true) {
            SyntaxKind kind = tokens->first()->kind;
            if (kind == SyntaxKind::CONST_KW || kind == SyntaxKind::INT_KW) {
                varDecl();
            } else if (statementFirst.count(kind)) {
                statement();
            } else {
                break;
            }
        }
        CHECK_TOKEN(SyntaxKind::R_CURLY, ErrorKind::UNDEFINED_ERROR);
        builder->finishNode(new BlockNode());
    }

    void statement() {
        builder->startNode(SyntaxKind::STMT);
        SyntaxKind kind = tokens->first()->kind;
        if (exprFirst.count(kind)) {
            int point = builder->checkPoint();
            expr();
            kind = tokens->first()->kind;
            if (kind == SyntaxKind::ASSIGN) {
                assignStmt(point);
            } else {
                exprStmt(point);
            }
        } else if (kind == SyntaxKind::SEMICOLON) {
            builder->startNode(SyntaxKind::EXPR_STMT);
            terminalSymbol();
            builder->finishNode(new ExprStmtNode());
        } else if (kind == SyntaxKind::L_CURLY) {
            block();
        } else if (kind == SyntaxKind::IF_KW) {
            ifStmt();
        } else if (kind == SyntaxKind::WHILE_KW) {
            whileStmt();
        } else if (kind == SyntaxKind::BREAK_KW) {
            breakStmt();
        } else if (kind == SyntaxKind::CONTINUE_KW) {
            continueStmt();
        } else if (kind == SyntaxKind::RETURN_KW) {
            returnStmt();
        } else if (kind == SyntaxKind::PRINTF_KW) {
            printfStmt();
        } else {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        builder->finishNode(new StatementNode());
    }

    void exprStmt(int point) {
        builder->startNodeAt(point, SyntaxKind::EXPR_STMT);
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new ExprStmtNode());
    }

    void assignStmt(int point) {
        builder->startNodeAt(point, SyntaxKind::ASSIGN_STMT);
        terminalSymbol();
        SyntaxKind kind = tokens->first()->kind;
        if (kind == SyntaxKind::GETINT_KW) {
            terminalSymbol();
            CHECK_TOKEN(SyntaxKind::L_PAREN, ErrorKind::UNDEFINED_ERROR);
            CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        } else if (exprFirst.count(kind)) {
            expr();
        } else {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new AssignStmtNode());
    }

    void printfStmt() {
        builder->startNode(SyntaxKind::PRINTF_STMT);
        CHECK_TOKEN(SyntaxKind::PRINTF_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::L_PAREN, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::STRING, ErrorKind::UNDEFINED_ERROR);
        while (tokens->first()->kind == SyntaxKind::COMMA) {
            terminalSymbol();
            expr();
        }
        CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new PrintfStmtNode());
    }

    void returnStmt() {
        builder->startNode(SyntaxKind::RETURN_STMT);
        CHECK_TOKEN(SyntaxKind::RETURN_KW, ErrorKind::UNDEFINED_ERROR);
        if (exprFirst.count(tokens->first()->kind)) {
            expr();
        }
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new ReturnStmtNode());
    }

    void continueStmt() {
        builder->startNode(SyntaxKind::CONTINUE_STMT);
        CHECK_TOKEN(SyntaxKind::CONTINUE_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new ContinueStmtNode);
    }

    void breakStmt() {
        builder->startNode(SyntaxKind::BREAK_STMT);
        CHECK_TOKEN(SyntaxKind::BREAK_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new BreakStmtNode);
    }

    void ifStmt() {
        builder->startNode(SyntaxKind::IF_STMT);
        CHECK_TOKEN(SyntaxKind::IF_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::L_PAREN, ErrorKind::UNDEFINED_ERROR);
        expr();
        CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        statement();
        if (tokens->first()->kind == SyntaxKind::ELSE_KW) {
            terminalSymbol();
            statement();
        }
        builder->finishNode(new IfStmtNode());
    }

    void whileStmt() {
        builder->startNode(SyntaxKind::WHILE_STMT);
        CHECK_TOKEN(SyntaxKind::WHILE_KW, ErrorKind::UNDEFINED_ERROR);
        CHECK_TOKEN(SyntaxKind::L_PAREN, ErrorKind::UNDEFINED_ERROR);
        expr();
        CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        statement();
        builder->finishNode(new WhileStmtNode);
    }

    void expr() {
        builder->startNode(SyntaxKind::EXPR);
        binExpr(0);
        builder->finishNode(new ExprNode());
    }

    void binExpr(int minPri) {
        int point = builder->checkPoint();
        prefixExpr();
        SyntaxKind kind = tokens->first()->kind;
        while (binOP2priority.count(kind)) {
            int priority = binOP2priority[kind];
            if (priority > minPri) {
                builder->startNodeAt(point, SyntaxKind::BIN_EXPR);
                terminalSymbol();
                binExpr(priority);
                builder->finishNode(new BinExprNode());
            } else {
                break;
            }
            kind = tokens->first()->kind;
        }
    }

    void prefixExpr() {
        builder->startNode(SyntaxKind::PREFIX_EXPR);
        SyntaxKind kind = tokens->first()->kind;
        if (prefixOPTokens.count(kind)) {
            prefixOP();
            kind = tokens->first()->kind;
        }
        if (kind == SyntaxKind::L_PAREN) {
            terminalSymbol();
            expr();
            CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        } else if (kind == SyntaxKind::INTEGER) {
            literalExpr();
        } else if (kind == SyntaxKind::IDENT) {
            if (tokens->nthToken(1)->kind == SyntaxKind::L_PAREN) {
                funcExpr();
            } else {
                varExpr();
            }
        } else {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        builder->finishNode(new PrefixExprNode());
    }

    void literalExpr() {
        builder->startNode(SyntaxKind::LITERAL_EXPR);
        CHECK_TOKEN(SyntaxKind::INTEGER, ErrorKind::UNDEFINED_ERROR);
        builder->finishNode(new LiteralExprNode());
    }

    void varExpr() {
        builder->startNode(SyntaxKind::VAR_EXPR);
        if (tokens->first()->kind != SyntaxKind::IDENT) {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        terminalSymbol();
        while (tokens->first()->kind == SyntaxKind::L_BRACKET) {
            terminalSymbol();
            expr();
            CHECK_TOKEN(SyntaxKind::R_BRACKET, ErrorKind::R_BRACKET_LACKED);
        }
        builder->finishNode(new VarExprNode());
    }

    void funcExpr() {
        builder->startNode(SyntaxKind::FUNC_EXPR);
        if (tokens->first()->kind != SyntaxKind::IDENT) {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        terminalSymbol();
        if (tokens->first()->kind != SyntaxKind::L_PAREN) {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        terminalSymbol();
        if (tokens->first()->kind == SyntaxKind::R_PAREN) {
            terminalSymbol();
        } else if (exprFirst.count(tokens->first()->kind)) {
            funcArgs();
            CHECK_TOKEN(SyntaxKind::R_PAREN, ErrorKind::R_PAREN_LACKED);
        } else {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        builder->finishNode(new FuncExprNode());
    }

    void funcArgs() {
        builder->startNode(SyntaxKind::FUNC_ARGS);
        expr();
        while (tokens->first()->kind == SyntaxKind::COMMA) {
            terminalSymbol();
            expr();
        }
        builder->finishNode(new FuncArgsNode());
    }

    void prefixOP() {
        builder->startNode(SyntaxKind::PREFIX_OP);
        while (prefixOPTokens.count(tokens->first()->kind)) {
            terminalSymbol();
        }
        builder->finishNode(new PrefixOPNode());
    }

    void varDecl() {
        builder->startNode(SyntaxKind::VAR_DECL);
        if (tokens->first()->kind == SyntaxKind::CONST_KW) {
            terminalSymbol();
        }
        CHECK_TOKEN(SyntaxKind::INT_KW, ErrorKind::UNDEFINED_ERROR);
        varDef();
        while (tokens->first()->kind == SyntaxKind::COMMA) {
            terminalSymbol();
            varDef();
        }
        CHECK_TOKEN(SyntaxKind::SEMICOLON, ErrorKind::SEMICOLON_LACKED);
        builder->finishNode(new VarDeclNode());
    }

    void varDef() {
        builder->startNode(SyntaxKind::VAR_DEF);
        CHECK_TOKEN(SyntaxKind::IDENT, ErrorKind::UNDEFINED_ERROR);
        while (tokens->first()->kind == SyntaxKind::L_BRACKET) {
            terminalSymbol();
            expr();
            CHECK_TOKEN(SyntaxKind::R_BRACKET, ErrorKind::R_BRACKET_LACKED);
        }
        if (tokens->first()->kind == SyntaxKind::ASSIGN) {
            terminalSymbol();
            initVal();
        }
        builder->finishNode(new VarDefNode());
    }

    void initVal() {
        builder->startNode(SyntaxKind::INIT_VAL);
        SyntaxKind kind = tokens->first()->kind;
        if (exprFirst.count(kind)) {
            expr();
        } else if (kind == SyntaxKind::L_CURLY) {
            terminalSymbol();
            kind = tokens->first()->kind;
            if (kind == SyntaxKind::L_CURLY || exprFirst.count(kind)) {
                initVal();
                while (tokens->first()->kind == SyntaxKind::COMMA) {
                    terminalSymbol();
                    initVal();
                }
                CHECK_TOKEN(SyntaxKind::R_CURLY, ErrorKind::UNDEFINED_ERROR);
            } else if (kind == SyntaxKind::R_CURLY) {
                terminalSymbol();
            } else {
                error(ErrorKind::UNDEFINED_ERROR);
            }
        } else {
            error(ErrorKind::UNDEFINED_ERROR);
            return;
        }
        builder->finishNode(new InitValNode());
    }

    void terminalSymbol() {
        builder->terminalSymbol(KIND, &tokens->first()->content, tokens->first()->line);
        tokens->bump();
    }

public:
    explicit Parser(TokenSource *tokens) {
        this->tokens = tokens;
        builder = new TreeBuilder();
    }

    Node* parse() {
        compileUnit();
        return builder->root();
    }
};
#endif //SYSYCOMPILER_PARSER_H
