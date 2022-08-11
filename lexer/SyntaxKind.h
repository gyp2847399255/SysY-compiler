//
// Created by gyp on 2021/9/25.
//

#ifndef SYSYCOMPILER_SYNTAXKIND_H
#define SYSYCOMPILER_SYNTAXKIND_H
#include <bits/stdc++.h>
using namespace std;

enum class SyntaxKind {
    EOF_CHAR,
    LINE_COMMENT,
    BLOCK_COMMENT,
    ERROR,
    COMMA,

    SEMICOLON,  // ;
    PLUS,
    MINUS,
    MULTI,
    DIV,
    MOD,
    NOT_LOGIC,
    ASSIGN,
    EQ, // ==
    NEQ,
    AND_LOGIC, // &&
    OR_LOGIC, // ||
    L_CURLY, // {
    R_CURLY, // }
    L_BRACKET, // [
    R_BRACKET, // ]
    L_PAREN, // (
    R_PAREN, // )
    GE,
    LE,
    GT,
    LT,
    IDENT,
    INTEGER,
    STRING,

    MAIN_KW,
    GETCHAR_KW,
    PRINTF_KW,
    INT_KW,
    VOID_KW,
    IF_KW,
    ELSE_KW,
    WHILE_KW,
    BREAK_KW,
    CONTINUE_KW,
    RETURN_KW,
    CONST_KW,

    LITERAL_EXPR,
    VAR_EXPR,
    FUNC_ARGS,
    FUNC_EXPR,
    PREFIX_EXPR,
    PREFIX_OP,
    BIN_EXPR,
    EXPR,

    EXPR_STMT,
    BLOCK,
    IF_STMT,
    WHILE_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    RETURN_STMT,
    ASSIGN_STMT,
    PRINTF_STMT,
    STMT,

    INIT_VAL,
    VAR_DEF,
    VAR_DECL,

    FUNC_TYPE,
    FUNC_PARAM,
    FUNC_PARAMS,

    FUNC_DEF,

    COMP_UNIT,
};

set<SyntaxKind> prefixOPTokens = {
        SyntaxKind::PLUS,
        SyntaxKind::MINUS,
        SyntaxKind::NOT_LOGIC
};

map<string, SyntaxKind> string2KeyWord = {
        {"main", SyntaxKind::MAIN_KW},
        {"const", SyntaxKind::CONST_KW},
        {"int", SyntaxKind::INT_KW},
        {"break", SyntaxKind::BREAK_KW},
        {"continue", SyntaxKind::CONTINUE_KW},
        {"if", SyntaxKind::IF_KW},
        {"else", SyntaxKind::ELSE_KW},
        {"while", SyntaxKind::WHILE_KW},
        {"getchar", SyntaxKind::GETCHAR_KW},
        {"printf", SyntaxKind::PRINTF_KW},
        {"return", SyntaxKind::RETURN_KW},
        {"void", SyntaxKind::VOID_KW}
};

map<SyntaxKind, string> syntaxKind2Name = {
        {SyntaxKind::IDENT, "IDENFR"},
        {SyntaxKind::INTEGER, "INTCON"},
        {SyntaxKind::STRING, "STRCON"},
        {SyntaxKind::MAIN_KW, "MAINTK"},
        {SyntaxKind::CONST_KW, "CONSTTK"},
        {SyntaxKind::INT_KW, "INTTK"},
        {SyntaxKind::BREAK_KW, "BREAKTK"},
        {SyntaxKind::CONTINUE_KW, "CONTINUETK"},
        {SyntaxKind::IF_KW, "IFTK"},
        {SyntaxKind::ELSE_KW, "ELSETK"},
        {SyntaxKind::NOT_LOGIC, "NOT"},
        {SyntaxKind::AND_LOGIC, "AND"},
        {SyntaxKind::OR_LOGIC, "OR"},
        {SyntaxKind::WHILE_KW, "WHILETK"},
        {SyntaxKind::GETCHAR_KW, "GETCHARTK"},
        {SyntaxKind::PRINTF_KW, "PRINTFTK"},
        {SyntaxKind::RETURN_KW, "RETURNTK"},
        {SyntaxKind::PLUS, "PLUS"},
        {SyntaxKind::MINUS, "MINU"},
        {SyntaxKind::VOID_KW, "VOIDTK"},
        {SyntaxKind::MULTI, "MULT"},
        {SyntaxKind::DIV, "DIV"},
        {SyntaxKind::MOD, "MOD"},
        {SyntaxKind::LT, "LSS"},
        {SyntaxKind::LE, "LEQ"},
        {SyntaxKind::GT, "GRE"},
        {SyntaxKind::GE, "GEQ"},
        {SyntaxKind::EQ, "EQL"},
        {SyntaxKind::NEQ, "NEQ"},
        {SyntaxKind::ASSIGN, "ASSIGN"},
        {SyntaxKind::SEMICOLON, "SEMICN"},
        {SyntaxKind::COMMA, "COMMA"},
        {SyntaxKind::L_PAREN, "LPARENT"},
        {SyntaxKind::R_PAREN, "RPARENT"},
        {SyntaxKind::L_BRACKET, "LBRACK"},
        {SyntaxKind::R_BRACKET, "RBRACK"},
        {SyntaxKind::L_CURLY, "LBRACE"},
        {SyntaxKind::R_CURLY, "RBRACE"},

        {SyntaxKind::ERROR, "ERROR"},

        {SyntaxKind::COMP_UNIT, "COMP_UNIT"},
        {SyntaxKind::VAR_DECL, "VAR_DECL"},
        {SyntaxKind::VAR_DEF, "VAR_DEF"},
        {SyntaxKind::INIT_VAL, "INIT_VAL"},
        {SyntaxKind::FUNC_DEF, "FUNC_DEF"},
        {SyntaxKind::FUNC_TYPE, "FUNC_TYPE"},
        {SyntaxKind::FUNC_PARAMS, "FUNC_PARAMS"},
        {SyntaxKind::FUNC_PARAM, "FUNC_PARAM"},
        {SyntaxKind::BLOCK, "BLOCK"},

        {SyntaxKind::STMT, "STMT"},
        {SyntaxKind::ASSIGN_STMT, "ASSIGN_STMT"},
        {SyntaxKind::EXPR_STMT, "EXPR_STMT"},
        {SyntaxKind::IF_STMT, "IF_STMT"},
        {SyntaxKind::WHILE_STMT, "WHILE_STMT"},
        {SyntaxKind::BREAK_STMT, "BREAK_STMT"},
        {SyntaxKind::CONTINUE_STMT, "CONTINUE_STMT"},
        {SyntaxKind::RETURN_STMT, "RETURN_STMT"},
        {SyntaxKind::PRINTF_STMT, "PRINTF_STMT"},

        {SyntaxKind::EXPR, "EXPR"},
        {SyntaxKind::FUNC_EXPR, "FUNC_EXPR"},
        {SyntaxKind::BIN_EXPR, "BIN_EXPR"},
        {SyntaxKind::LITERAL_EXPR, "LITERAL_EXPR"},
        {SyntaxKind::PREFIX_EXPR, "PREFIX_EXPR"},
        {SyntaxKind::VAR_EXPR, "VAR_EXPR"},
        {SyntaxKind::FUNC_ARGS, "FUNC_AGRS"},
};

map<SyntaxKind, int> binOP2priority = {
        {SyntaxKind::OR_LOGIC, 10},
        {SyntaxKind::AND_LOGIC, 20},
        {SyntaxKind::EQ, 30},
        {SyntaxKind::NEQ, 30},
        {SyntaxKind::GT, 40},
        {SyntaxKind::GE, 40},
        {SyntaxKind::LE, 40},
        {SyntaxKind::LT, 40},
        {SyntaxKind::PLUS, 50},
        {SyntaxKind::MINUS, 50},
        {SyntaxKind::MULTI, 60},
        {SyntaxKind::DIV, 60},
        {SyntaxKind::MOD, 60}
};

set<SyntaxKind> exprFirst = {
        SyntaxKind::IDENT,
        SyntaxKind::L_PAREN,
        SyntaxKind::INTEGER,
        SyntaxKind::PLUS,
        SyntaxKind::MINUS,
        SyntaxKind::NOT_LOGIC,
};

set<SyntaxKind> statementFirst = {
        SyntaxKind::SEMICOLON,
        SyntaxKind::L_CURLY,
        SyntaxKind::IF_KW,
        SyntaxKind::WHILE_KW,
        SyntaxKind::BREAK_KW,
        SyntaxKind::CONTINUE_KW,
        SyntaxKind::RETURN_KW,
        SyntaxKind::PRINTF_KW,

        SyntaxKind::IDENT,
        SyntaxKind::L_PAREN,
        SyntaxKind::INTEGER,
        SyntaxKind::PLUS,
        SyntaxKind::MINUS,
        SyntaxKind::NOT_LOGIC,
};

SyntaxKind judgeIdent(string *ident) {
    if (string2KeyWord.count(*ident)) {
        return string2KeyWord[*ident];
    }
    return SyntaxKind::IDENT;
}

#endif // SYSYCOMPILER_SYNTAXKIND_H