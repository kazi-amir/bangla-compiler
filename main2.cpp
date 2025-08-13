#include <bits/stdc++.h>
using namespace std;

// Enum to represent different types of tokens
enum class TokenType {
    KEYWORD, IDENTIFIER, INTEGER_LITERAL, STRING_LITERAL,
    OPERATOR, PUNCTUATION, END_OF_FILE, UNKNOWN
};

map<TokenType, string> tokenTypeToString = {
    {TokenType::KEYWORD, "KEYWORD"}, {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::INTEGER_LITERAL, "INTEGER_LITERAL"}, {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::OPERATOR, "OPERATOR"}, {TokenType::PUNCTUATION, "PUNCTUATION"},
    {TokenType::END_OF_FILE, "END_OF_FILE"}, {TokenType::UNKNOWN, "UNKNOWN"}
};


int main() {
    ifstream inputFile("code.bgl");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file 'code.bgl'. Please create it and place your code inside." << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string sourceCode = buffer.str();
    cout << sourceCode << endl;

    try {
        


        cout << "Compilation Successfull" << endl;
    } catch (const runtime_error& e) {
        cerr << "Compilation Error: " << e.what() << endl;
        return 1;
    }
}