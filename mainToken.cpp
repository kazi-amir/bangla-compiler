#include <bits/stdc++.h>
using namespace std;
#include <cctype>

map<string, string> token{
    {"shuru", "#include <bits/stdc++.h>\nusing namespace std;\n\nint main(){ \n"},
    {"purno", "int "}, {"vogno", "double "}, {"bakko", "string "}, {"borno", "char "}, 
    {"jodi", "if "}, {"nahole", "else "}, {"loop", "for "},
    {"shesh", "return 0; \n} "}, {";", "; \n"}, {"{", "{ \n"}, {"}", "} \n"}, 
    {"dekhao", "cout << "}
};

// --- Tokenizer Function ---
vector<string> tokenize(const string& sourceCode) {
    vector<string> tokens;
    size_t pos = 0;

    while (pos < sourceCode.length()) {
        char currentChar = sourceCode[pos];

        // 1. Skip whitespace characters
        if (isspace(currentChar)) {
            pos++;
            continue;
        }

        // NEW: 2. Handle string literals
        // If a double quote is found, everything until the next double quote is one token.
        if (currentChar == '"') {
            string strLiteral;
            strLiteral += currentChar; // Add the opening quote
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
            tokens.push_back(strLiteral);
            continue; // Go to the beginning of the main loop to find the next token
        }

        // 3. Tokenize words (keywords, identifiers)
        if (isalpha(currentChar) || currentChar == '_') {
            string word;
            while (pos < sourceCode.length() && (isalnum(sourceCode[pos]) || sourceCode[pos] == '_')) {
                word += sourceCode[pos];
                pos++;
            }
            tokens.push_back(word);
            continue;
        }

        // 4. Tokenize numbers
        if (isdigit(currentChar)) {
            string number;
            while (pos < sourceCode.length() && isdigit(sourceCode[pos])) {
                number += sourceCode[pos];
                pos++;
            }
            tokens.push_back(number);
            continue;
        }

        // 5. Tokenize operators and punctuation (with look-ahead)
        if (pos + 1 < sourceCode.length()) {
            string twoCharOp = sourceCode.substr(pos, 2);
            if (twoCharOp == "==" || twoCharOp == "!=" || twoCharOp == ">=" || twoCharOp == "<=" ||
                twoCharOp == "+=" || twoCharOp == "-=" || twoCharOp == "*=" || twoCharOp == "/=")
            {
                tokens.push_back(twoCharOp);
                pos += 2;
                continue;
            }
        }

        // Handle single-character tokens
        tokens.push_back(string(1, currentChar));
        pos++;
    }

    return tokens;
}

// --- Main Function ---
int main() {
    string filename = "code.bgl"; //The file to be opened
    ifstream inputFile(filename); //read file with name filename

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return 1;
    }

    string sourceCode((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    vector<string> tokens = tokenize(sourceCode);

    ofstream outputFile("output.cpp");
    if (outputFile.is_open()) {
        int tabs = 0;
        for(int i = 0; i<tokens.size(); i++){
            if((tokens[i]=="{" || tokens[i]=="shuru")){
                tabs++;
            }
            if(tokens[i]=="}"){
                tabs--;
            }
            if(i>0 && (tokens[i-1] == "shuru" || tokens[i-1] == "{" || tokens[i-1] == ";" || tokens[i] == "}" || tokens[i-1] == "}")){
                for(int j = 0; j < tabs; j++){
                    outputFile << "\t";
                }
            }
            
            if(token.find(tokens[i]) != token.end()){
                outputFile << token[tokens[i]];
            }
            else{
                outputFile << tokens[i];
            }
            
        }

        outputFile.close();
        // cout << "Successfully saved generated code to output.cpp" << endl;
    } else {
        cerr << "Error: Could not open file output.cpp for writing." << endl;
    }

    return 0;
}
