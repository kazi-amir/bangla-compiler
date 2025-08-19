#include<bits/stdc++.h>
using namespace std;

map<string, string> token{
    {"shuru", "#include <bits/stdc++.h>\nusing namespace std;\n\nint main(){ \n"},
    {"purno", "int "}, {"vogno", "double "}, {"bakko", "string "}, {"borno", "char "}, 
    {"jodi", "if "}, {"nahole", "else "}, {"loop", "for "},
    {"shesh", "return 0; \n} "}, {"#", "; \n"}, {"{", "{ \n"}, {"}", "} \n"}, 
    {"dekhao", "printf"}, {";", "; "}, {"nao", "scanf"}
};

void compileCode(const vector<string> &tokens){
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
            if(i>0 && (tokens[i-1] == "shuru" || tokens[i-1] == "{" || tokens[i-1] == "#"  || 
                tokens[i] == "}" || tokens[i-1] == "}" || tokens[i-1][0] == '/')){
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

    string compileCode = "cd \"/home/amir/Compiler Lab/bangla-compiler/\" && g++ output.cpp -o output && \"/home/amir/Compiler Lab/bangla-compiler/output\"";
    system(compileCode.c_str());
}
