//
// Created by gyp on 2021/10/14.
//

#ifndef SYSYCOMPILER_ERRORKIND_H
#define SYSYCOMPILER_ERRORKIND_H
#include <bits/stdc++.h>
using namespace std;

enum class ErrorKind {
    INVALID_CHARACTER,
    REDEFINE_IDENT,
    UNDEFINED_IDENT,
    PARAMS_NUM_UNMATCHED,
    PARAM_TYPE_UNMATCHED,
    VOID_FUNC_RETURN_INTEGER,
    INT_FUNC_RETURN_LACKED,
    CONSTANT_ASSIGNED,
    SEMICOLON_LACKED,
    R_PAREN_LACKED,
    R_BRACKET_LACKED,
    FORMAT_CHAR_UNMATCHED,
    BREAK_CONTINUE_OUT_LOOP,
    UNDEFINED_ERROR,
    CORRECT
};

map<ErrorKind, char> errorKind2Code = {
        {ErrorKind::INVALID_CHARACTER, 'a'},
        {ErrorKind::REDEFINE_IDENT, 'b'},
        {ErrorKind::UNDEFINED_IDENT, 'c'},
        {ErrorKind::PARAMS_NUM_UNMATCHED, 'd'},
        {ErrorKind::PARAM_TYPE_UNMATCHED, 'e'},
        {ErrorKind::VOID_FUNC_RETURN_INTEGER, 'f'},
        {ErrorKind::INT_FUNC_RETURN_LACKED, 'g'},
        {ErrorKind::CONSTANT_ASSIGNED, 'h'},
        {ErrorKind::SEMICOLON_LACKED, 'i'},
        {ErrorKind::R_PAREN_LACKED, 'j'},
        {ErrorKind::R_BRACKET_LACKED, 'k'},
        {ErrorKind::FORMAT_CHAR_UNMATCHED, 'l'},
        {ErrorKind::BREAK_CONTINUE_OUT_LOOP, 'm'},
        {ErrorKind::UNDEFINED_ERROR, 'n'}
};

vector<pair<ErrorKind, int>> errorList;
#endif //SYSYCOMPILER_ERRORKIND_H
