#include<bits/stdc++.h>
using namespace std;

//Lexical Analysis and Tokenizer function
vector<string> Lexer(const string& sourceCode) {
    vector<string> tokens;
    size_t pos = 0;

    while (pos < sourceCode.length()) {
        char currentChar = sourceCode[pos];

        //Skip whitespace characters
        if (isspace(currentChar)) {
            pos++;
            continue;
        }

        //Handle string literals
        // If a double quote is found, everything until the next double quote is one token.
        if (currentChar == '"') {
            string strLiteral;
            strLiteral += currentChar; // Add the opening double quote
            pos++;                   // Move past the opening quote

            // Keep reading until the closing quote is found or we reach the end of the file
            while (pos < sourceCode.length()) {
                strLiteral += sourceCode[pos];
                if (sourceCode[pos] == '"') {
                    pos++; // Move past the closing quote
                    break; // Exit the inner string-reading loop
                }
                pos++;
            }
            tokens.push_back(strLiteral); //store the token extracted to tokens data structure
            continue; // continue to next token
        }

        //Single line comment handle
        string comment;
        if(currentChar == '/' && pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '/') {
            // Skip to the end of the line
            while (pos < sourceCode.length() && sourceCode[pos] != '\n') {
                comment += sourceCode[pos];
                pos++;
            }
            comment+="\n";
            tokens.push_back(comment); //store the comment token
            continue; // continue to next token
        }

        //Tokenize words (keywords and identifiers)
        if (isalpha(currentChar) || currentChar == '_') {
            string word;
            //isalnum is to check if the character is alphabet or numeric(alphanumeric)
            while (pos < sourceCode.length() && (isalnum(sourceCode[pos]) || sourceCode[pos] == '_')) {
                word += sourceCode[pos];
                pos++;
            }
            tokens.push_back(word);
            continue; // continue to next token
        }

        //Tokenize numbers
        if (isdigit(currentChar)) {
            string number;
            while (pos < sourceCode.length() && isdigit(sourceCode[pos])) {
                number += sourceCode[pos];
                pos++;
            }
            tokens.push_back(number);
            continue; //continue to next token
        }

        //Tokenize operators and punctuation
        if (pos + 1 < sourceCode.length()) {
            string twoCharOp = sourceCode.substr(pos, 2);
            if (twoCharOp == "==" || twoCharOp == "!=" || twoCharOp == ">=" || twoCharOp == "<=" ||
                twoCharOp == "+=" || twoCharOp == "-=" || twoCharOp == "*=" || twoCharOp == "/=" || 
                twoCharOp == "++" || twoCharOp == "--")
            {
                tokens.push_back(twoCharOp);
                pos += 2;
                continue;
            }
        }

        // Handle single-character tokens(+, -, ;, <, >, etc)
        tokens.push_back(string(1, currentChar));
        pos++;
    }

    return tokens;
}
