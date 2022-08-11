#include <bits/stdc++.h>
using namespace std;

char cmd[1000];
int main() {
    for (int i = 1; i <= 82; i++) {
        sprintf(cmd, "touch tmp.txt");
        system(cmd);
        sprintf(cmd, "echo 'int getint() {\n\
	int x = 0, c = 0, minus = 0;\n\
	while((c < 48 || c > 57) && c != 45) {\n\
		c = getchar();\n\
	}\n\
	if (c == 45) {\n\
		minus = 1;\n\
		c = getchar();\n\
	}\n\
	while(c >= 48 && c <= 57) {\n\
		x = x * 10 + c - 48;\n\
		c = getchar();\n\
	}\n\
	if (minus) {\n\
		x = -x;\n\
	}\n\
	return x;\n\
}' > tmp.txt");
        system(cmd);
        sprintf(cmd, "cat testfile%d.txt >> tmp.txt", i);
        system(cmd);
        sprintf(cmd, "rm testfile%d.txt", i);
        system(cmd);
        sprintf(cmd, "mv tmp.txt testfile%d.txt", i);
        system(cmd);
    }
    return 0;
}


