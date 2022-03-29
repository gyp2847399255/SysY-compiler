#ifndef SYSYCOMPILER_SOURCE_H
#define SYSYCOMPILER_SOURCE_H
#include <bits/stdc++.h>
using namespace std;

class Source {
    string sources;
    vector<int>line;
public:
    explicit Source(string *fileName) {
        sources = "";
        line.clear();
        ifstream myFile(*fileName);
        if (!myFile.is_open()) {
            cout << "Can't open" << endl;
        } else {
            string tmp;
            line.push_back(0);
            while (getline(myFile, tmp)) {
                sources += tmp;
                sources += '\n';
                line.push_back(int(sources.length()));
            }
        }
        myFile.close();
    }

    int getLine(int num) {
        int ret;
        for (int i = 0; i < line.size(); i++) {
            if (num < line[i]) {
                ret = i;
                break;
            }
        }
        return ret;
    }

    string::iterator getBegin() {
        return sources.begin();
    }

    string::iterator getEnd() {
        return sources.end();
    }
};


#endif //SYSYCOMPILER_SOURCE_H
