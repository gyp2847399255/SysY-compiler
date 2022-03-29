#include <bits/stdc++.h>
using namespace std;

char cmd[1000];
vector<int> vct;
int main(int argc, char **argv) {
    bool ir = false;
    bool debug = false;
    if (argc > 1 && strcmp(argv[1], "ir") == 0) {
        ir = true;
        if (argc > 2) {
            debug = true;
            for (int i = 2; i < argc; i++) {
                vct.push_back(atoi(argv[i]));
            }
        }
    } else if (argc > 1) {
        debug = true;
        for (int i = 1; i < argc; i++) {
            vct.push_back(atoi(argv[i]));
        }
    }
    if (!debug) {
        for (int i = 1; i <= 82; i++) {
            vct.push_back(i);
        }
    }
    sprintf(cmd, "rm result.txt");
    system(cmd);
    sprintf(cmd, "g++ ../main.cpp -o compiler -O2");
    system(cmd);
    if (ir) {
        sprintf(cmd, "javac Main.java");
        system(cmd);
    }
	for (auto i : vct) {
        cout << "testing " << i << endl;
		sprintf(cmd, "cp ../test/testfile%d.txt testfile.txt", i);
		system(cmd);
		sprintf(cmd, "./compiler");
		system(cmd);
		sprintf(cmd, "cp ../test/input%d.txt input.txt", i);
		system(cmd);
        if (ir) {
            sprintf(cmd, "java Main > output.txt");
            system(cmd);
        } else {
            sprintf(cmd, "java -jar Mars-Compile-2021.jar mips.txt < input.txt "
                         "> output.txt && sed -i '1,2d' output.txt");
            system(cmd);
        }
        sprintf(cmd, "echo 'Testcase%d\n' >> result.txt", i);
        system(cmd);
        sprintf(cmd, "diff -b ../test/output%d.txt output.txt >> result.txt", i);
        system(cmd);
	}
	if (!debug) {
        if (ir) {
            sprintf(cmd, "rm ir.txt testfile.txt data.txt input.txt error.txt output.txt compiler");
            system(cmd);
        } else {
            sprintf(cmd, "rm mips.txt testfile.txt input.txt error.txt output.txt compiler");
            system(cmd);
        }
    }
    if (ir) {
        sprintf(cmd, "rm *.class");
        system(cmd);
    }
	return 0;
}
