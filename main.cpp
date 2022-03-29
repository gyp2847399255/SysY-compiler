#include "lib.h"
using namespace std;

vector<Token*> tokens;
Node *root;
int main() {
    ofstream outputFile("error.txt");
    string inputFile = "testfile.txt";
    source = new Source(&inputFile);
    Lexer lexer(source);
    tokens = lexer.tokenize();
    if (lexer.hasError()) {
        lexer.printError();
        return 0;
    }
    TokenSource tokenSource(&tokens);
    Parser parser(&tokenSource);
    root = parser.parse();
    //root->print(&outputFile);
    root->checkError(new ErrorCheckCtx(), new ErrorCheckRet());
    if (!errorList.empty()) {
        for (auto & i : errorList) {
            outputFile << i.second << " " << errorKind2Code[i.first] << endl;
        }
        outputFile.close();
        return 0;
    }
    root->buildIR(new BuildIRCtx, new BuildIRRet);
    return 0;
}
