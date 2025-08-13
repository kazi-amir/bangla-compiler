#include <bits/stdc++.h>
using namespace std;

map<string, string> token{
    {"shuru", "#include <bits/stdc++.h>\nusing namespace std;\n\nint main(){\n"},
    {"purno", "int"}, {"vogno", "double"}, {"bakko", "string"}, {"borno", "char"}, 
    {"jodi", "if"}, {"nahole_jodi", "else if"}, {"nahole", "else"}, {"loop", "for"},
    {"shesh", "return 0;\n}"}, {";", ";\n"}, {"{", "{\n"}, {"}", "}\n"}
};

string findToken(string &s){
    return token[s];
}

// int main(){
    
// }