//
// Created by gyp on 2021/11/18.
//

#ifndef SYSYCOMPILER_REGISTERALLOCATE_H
#define SYSYCOMPILER_REGISTERALLOCATE_H
class RegisterAllocate {
    const static int registerNumber = 18;
public:
    static int allocateRegister(map<string, int> *weight, map<int, string> *cur, string *var) {
        bool vis[registerNumber];
        memset(vis, 0, sizeof vis);
        for (auto & i : *cur) {
            vis[i.first - 8] = true;
        }
        for (int i = 0; i < registerNumber; i++) {
            if (!vis[i]) {
                return i + 8;
            }
        }
        int mn = (*weight)[*var];
        for (auto & i : *cur) {
            if ((*weight)[i.second] < mn) {
                mn = (*weight)[i.second];
            }
        }
        for (auto & i : *cur) {
            if ((*weight)[*var] == mn) {
                return i.first;
            }
        }
        return -1;
    }
};
#endif //SYSYCOMPILER_REGISTERALLOCATE_H
