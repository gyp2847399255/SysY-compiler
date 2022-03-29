//
// Created by gyp on 2021/9/23.
//

#ifndef SYSYCOMPILER_LIB_H
#define SYSYCOMPILER_LIB_H
#include <bits/stdc++.h>
#define IR false
#define OPT true
using namespace std;

bool exprResIsNumber(const string *res) {
    return regex_match(*res, regex("[+-]*[0-9]+"));
}
#include "lexer/lib.h"
#include "parser/lib.h"

#endif //SYSYCOMPILER_LIB_H
