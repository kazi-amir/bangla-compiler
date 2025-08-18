#include <bits/stdc++.h>
#include "lexer.cpp"
#include "compileBGL.cpp"
using namespace std;
#include <cctype>

int main() {
    string banglishCode = "code.bgl"; //Bangla source code file
    ifstream inputFile(banglishCode); //reads the Bangla source code file

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file '" << banglishCode << "'" << endl;
        return 1;
    }

    string sourceCode((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    vector<string> tokens = Lexer(sourceCode);
    compileCode(tokens);

    return 0;
}

