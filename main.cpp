#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <memory> // For unique_ptr
#include <stdexcept> // For runtime_error
#include <fstream> // For file output
#include <sstream> // For reading files

using namespace std;

// --- Tokenizer (Lexer) Section ---

// Enum to represent different types of tokens
enum class TokenType {
    KEYWORD, IDENTIFIER, INTEGER_LITERAL, STRING_LITERAL,
    OPERATOR, PUNCTUATION, END_OF_FILE, UNKNOWN
};

// Map to convert TokenType to string for printing
map<TokenType, string> tokenTypeToString = {
    {TokenType::KEYWORD, "KEYWORD"}, {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::INTEGER_LITERAL, "INTEGER_LITERAL"}, {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::OPERATOR, "OPERATOR"}, {TokenType::PUNCTUATION, "PUNCTUATION"},
    {TokenType::END_OF_FILE, "END_OF_FILE"}, {TokenType::UNKNOWN, "UNKNOWN"}
};

// Structure to represent a single token
struct Token {
    TokenType type;
    string value;
};

// The Lexer (Tokenizer)
class Lexer {
public:
    Lexer(const string& source) : source_(source), position_(0) {
        // Added "nahole_jodi" to the keywords list
        keywords_ = {
            {"shuru", true}, {"shesh", true}, {"jodi", true}, {"nahole_jodi", true},
            {"nahole", true}, {"dekhao", true}, {"rakho", true}, 
            {"shongkhya", true}, {"shobdo", true}
        };
    }

    Token getNextToken() {
        while (position_ < source_.length()) {
            char currentChar = source_[position_];

            // Skip whitespace
            if (isspace(currentChar)) {
                position_++;
                continue;
            }

            // Handle comments
            if (currentChar == '#') {
                // Multi-line comment: ## ... ##
                if (position_ + 1 < source_.length() && source_[position_ + 1] == '#') {
                    position_ += 2; // Skip opening '##'
                    while (position_ + 1 < source_.length() && !(source_[position_] == '#' && source_[position_ + 1] == '#')) {
                        position_++;
                    }
                    if (position_ + 1 < source_.length()) {
                        position_ += 2; // Skip closing '##'
                    }
                    continue; // Find the next token
                }
                // Single-line comment: # ... until newline
                else {
                    while (position_ < source_.length() && source_[position_] != '\n') {
                        position_++;
                    }
                    continue; // Find the next token
                }
            }

            // If not whitespace or comment, it's a token
            if (isdigit(currentChar)) return readInteger();
            if (isalpha(currentChar) || currentChar == '_') return readIdentifierOrKeyword();
            if (currentChar == '"') return readString();

            switch (currentChar) {
                case '+': case '-': case '*': case '/': case '=': case '<': case '>':
                    position_++;
                    if (position_ < source_.length() && source_[position_] == '=') {
                        char nextChar = source_[position_];
                        position_++;
                        return {TokenType::OPERATOR, string(1, currentChar) + string(1, nextChar)};
                    }
                    return {TokenType::OPERATOR, string(1, currentChar)};
                case ';': case '(': case ')': case '{': case '}': case ',':
                    position_++;
                    return {TokenType::PUNCTUATION, string(1, currentChar)};
            }

            position_++;
            return {TokenType::UNKNOWN, string(1, currentChar)};
        }

        return {TokenType::END_OF_FILE, ""};
    }

private:
    string source_;
    size_t position_;
    map<string, bool> keywords_;

    Token readInteger() {
        size_t start = position_;
        while (position_ < source_.length() && isdigit(source_[position_])) position_++;
        return {TokenType::INTEGER_LITERAL, source_.substr(start, position_ - start)};
    }

    Token readString() {
        position_++;
        size_t start = position_;
        while (position_ < source_.length() && source_[position_] != '"') position_++;
        string value = source_.substr(start, position_ - start);
        if (position_ < source_.length() && source_[position_] == '"') position_++;
        return {TokenType::STRING_LITERAL, value};
    }

    Token readIdentifierOrKeyword() {
        size_t start = position_;
        while (position_ < source_.length() && (isalnum(source_[position_]) || source_[position_] == '_')) position_++;
        string value = source_.substr(start, position_ - start);
        if (keywords_.count(value)) return {TokenType::KEYWORD, value};
        return {TokenType::IDENTIFIER, value};
    }
};

// --- Abstract Syntax Tree (AST) Section ---

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual string getType() const = 0;
};

struct ExpressionNode : public ASTNode {};
struct StatementNode : public ASTNode {};

struct NumberNode : public ExpressionNode {
    string value;
    NumberNode(string val) : value(val) {}
    string getType() const override { return "NumberNode"; }
};

struct StringNode : public ExpressionNode {
    string value;
    StringNode(string val) : value(val) {}
    string getType() const override { return "StringNode"; }
};

struct VariableNode : public ExpressionNode {
    string name;
    VariableNode(string n) : name(n) {}
    string getType() const override { return "VariableNode"; }
};

struct BinaryOpNode : public ExpressionNode {
    unique_ptr<ExpressionNode> left;
    Token op;
    unique_ptr<ExpressionNode> right;
    BinaryOpNode(unique_ptr<ExpressionNode> l, Token o, unique_ptr<ExpressionNode> r)
        : left(move(l)), op(o), right(move(r)) {}
    string getType() const override { return "BinaryOpNode"; }
};

struct VariableDeclarationNode : public StatementNode {
    string varType;
    string varName;
    unique_ptr<ExpressionNode> value;
    VariableDeclarationNode(string t, string n, unique_ptr<ExpressionNode> v)
        : varType(t), varName(n), value(move(v)) {}
    string getType() const override { return "VariableDeclarationNode"; }
};

struct PrintStatementNode : public StatementNode {
    unique_ptr<ExpressionNode> expression;
    PrintStatementNode(unique_ptr<ExpressionNode> expr) : expression(move(expr)) {}
    string getType() const override { return "PrintStatementNode"; }
};

struct BlockNode : public StatementNode {
    vector<unique_ptr<StatementNode>> statements;
    string getType() const override { return "BlockNode"; }
};

struct IfStatementNode : public StatementNode {
    unique_ptr<ExpressionNode> condition;
    unique_ptr<BlockNode> ifBlock;
    unique_ptr<StatementNode> elseNode; // Can be another IfStatementNode or a BlockNode
    IfStatementNode(unique_ptr<ExpressionNode> c, unique_ptr<BlockNode> ib, unique_ptr<StatementNode> en)
        : condition(move(c)), ifBlock(move(ib)), elseNode(move(en)) {}
    string getType() const override { return "IfStatementNode"; }
};

struct ProgramNode : public ASTNode {
    vector<unique_ptr<StatementNode>> statements;
    string getType() const override { return "ProgramNode"; }
};

// --- Parser Section ---

class Parser {
public:
    Parser(Lexer& lexer) : lexer_(lexer) {
        currentToken_ = lexer_.getNextToken();
    }

    unique_ptr<ProgramNode> parseProgram() {
        auto programNode = unique_ptr<ProgramNode>(new ProgramNode());
        eat(TokenType::KEYWORD, "shuru");

        while (currentToken_.type != TokenType::KEYWORD || currentToken_.value != "shesh") {
            programNode->statements.push_back(parseStatement());
        }
        
        eat(TokenType::KEYWORD, "shesh");
        return programNode;
    }

private:
    Lexer& lexer_;
    Token currentToken_;

    // Forward declaration for recursive use in parseStatement
    unique_ptr<StatementNode> parseIfStatement();

    void eat(TokenType type, const string& value = "") {
        if (currentToken_.type == type && (value.empty() || currentToken_.value == value)) {
            currentToken_ = lexer_.getNextToken();
        } else {
            throw runtime_error("Parser error: Unexpected token '" + currentToken_.value + "'");
        }
    }

    unique_ptr<ExpressionNode> parseFactor() {
        Token token = currentToken_;
        if (token.type == TokenType::INTEGER_LITERAL) {
            eat(TokenType::INTEGER_LITERAL);
            return unique_ptr<NumberNode>(new NumberNode(token.value));
        } else if (token.type == TokenType::STRING_LITERAL) {
            eat(TokenType::STRING_LITERAL);
            return unique_ptr<StringNode>(new StringNode(token.value));
        } else if (token.type == TokenType::IDENTIFIER) {
            eat(TokenType::IDENTIFIER);
            return unique_ptr<VariableNode>(new VariableNode(token.value));
        } else if (token.value == "(") {
            eat(TokenType::PUNCTUATION, "(");
            auto node = parseExpression();
            eat(TokenType::PUNCTUATION, ")");
            return node;
        }
        throw runtime_error("Parser error: Unexpected factor token '" + token.value + "'");
    }

    unique_ptr<ExpressionNode> parseTerm() {
        auto node = parseFactor();
        while (currentToken_.value == "*" || currentToken_.value == "/") {
            Token op = currentToken_;
            eat(TokenType::OPERATOR);
            node = unique_ptr<BinaryOpNode>(new BinaryOpNode(move(node), op, parseFactor()));
        }
        return node;
    }

    unique_ptr<ExpressionNode> parseExpression() {
        auto node = parseTerm();
        while (currentToken_.type == TokenType::OPERATOR) {
            Token op = currentToken_;
            eat(TokenType::OPERATOR);
            node = unique_ptr<BinaryOpNode>(new BinaryOpNode(move(node), op, parseTerm()));
        }
        return node;
    }
    
    unique_ptr<BlockNode> parseBlock() {
        eat(TokenType::PUNCTUATION, "{");
        auto block = unique_ptr<BlockNode>(new BlockNode());
        while (currentToken_.value != "}") {
            block->statements.push_back(parseStatement());
        }
        eat(TokenType::PUNCTUATION, "}");
        return block;
    }

    unique_ptr<StatementNode> parseStatement() {
        if (currentToken_.type == TokenType::KEYWORD) {
            if (currentToken_.value == "rakho") {
                return parseVariableDeclaration();
            } else if (currentToken_.value == "dekhao") {
                return parsePrintStatement();
            } else if (currentToken_.value == "jodi") {
                // Consume 'jodi' here before calling the parsing function
                eat(TokenType::KEYWORD, "jodi");
                return parseIfStatement();
            }
        }
        throw runtime_error("Parser error: Invalid statement starting with '" + currentToken_.value + "'");
    }

    unique_ptr<StatementNode> parseVariableDeclaration() {
        eat(TokenType::KEYWORD, "rakho");
        string type = currentToken_.value;
        eat(TokenType::KEYWORD);
        string name = currentToken_.value;
        eat(TokenType::IDENTIFIER);
        eat(TokenType::OPERATOR, "=");
        auto value = parseExpression();
        eat(TokenType::PUNCTUATION, ";");
        return unique_ptr<VariableDeclarationNode>(new VariableDeclarationNode(type, name, move(value)));
    }

    unique_ptr<StatementNode> parsePrintStatement() {
        eat(TokenType::KEYWORD, "dekhao");
        auto expr = parseExpression();
        eat(TokenType::PUNCTUATION, ";");
        return unique_ptr<PrintStatementNode>(new PrintStatementNode(move(expr)));
    }
};

// --- Parser method definitions that need forward declaration ---

unique_ptr<StatementNode> Parser::parseIfStatement() {
    // This function is now called AFTER 'jodi' has been consumed.
    eat(TokenType::PUNCTUATION, "(");
    auto condition = parseExpression();
    eat(TokenType::PUNCTUATION, ")");
    auto ifBlock = parseBlock();

    unique_ptr<StatementNode> elseNode = nullptr;
    if (currentToken_.type == TokenType::KEYWORD) {
        if (currentToken_.value == "nahole_jodi") {
            eat(TokenType::KEYWORD, "nahole_jodi");
            // The next part is an "if" statement. We can recursively call this function.
            elseNode = parseIfStatement();
        } else if (currentToken_.value == "nahole") {
            eat(TokenType::KEYWORD, "nahole");
            elseNode = parseBlock();
        }
    }
    
    return unique_ptr<IfStatementNode>(new IfStatementNode(move(condition), move(ifBlock), move(elseNode)));
}


// --- Code Generator Section ---

class CodeGenerator {
public:
    string generate(const ProgramNode* program) {
        string output = "#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n";
        for (const auto& stmt : program->statements) {
            output += generateStatement(stmt.get());
        }
        output += "    return 0;\n}\n";
        return output;
    }

private:
    string generateExpression(const ExpressionNode* expr) {
        if (auto node = dynamic_cast<const NumberNode*>(expr)) {
            return node->value;
        }
        if (auto node = dynamic_cast<const StringNode*>(expr)) {
            return "\"" + node->value + "\"";
        }
        if (auto node = dynamic_cast<const VariableNode*>(expr)) {
            return node->name;
        }
        if (auto node = dynamic_cast<const BinaryOpNode*>(expr)) {
            return "(" + generateExpression(node->left.get()) + " " + node->op.value + " " + generateExpression(node->right.get()) + ")";
        }
        throw runtime_error("CodeGenerator: Unknown expression type.");
    }

    string generateStatement(const StatementNode* stmt, int indent = 1) {
        string indentation(indent * 4, ' ');
        string code;
        if (auto node = dynamic_cast<const VariableDeclarationNode*>(stmt)) {
            string cppType = (node->varType == "shongkhya") ? "int" : "string";
            code += indentation + cppType + " " + node->varName + " = " + generateExpression(node->value.get()) + ";\n";
        } else if (auto node = dynamic_cast<const PrintStatementNode*>(stmt)) {
            code += indentation + "cout << " + generateExpression(node->expression.get()) + " << endl;\n";
        } else if (auto node = dynamic_cast<const IfStatementNode*>(stmt)) {
            // Handle the "if" part
            if (indent > 0) { // Don't indent for chained "else if"
                 code += indentation;
            }
            code += "if " + generateExpression(node->condition.get()) + " {\n";
            for(const auto& s : node->ifBlock->statements) {
                code += generateStatement(s.get(), indent + 1);
            }
            code += indentation + "}";

            // Handle the "else" or "else if" part
            if (node->elseNode) {
                code += " else ";
                // Check if the else part is another if-statement (for "else if")
                if (dynamic_cast<const IfStatementNode*>(node->elseNode.get())) {
                    code += generateStatement(node->elseNode.get(), 0); // indent 0 prevents extra spacing
                } 
                // Otherwise, it's a standard "else" block
                else {
                    code += "{\n";
                    auto elseBlock = dynamic_cast<const BlockNode*>(node->elseNode.get());
                    for(const auto& s : elseBlock->statements) {
                       code += generateStatement(s.get(), indent + 1);
                    }
                    code += indentation + "}\n";
                }
            } else {
                code += "\n";
            }
        }
        return code;
    }
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

    cout << "--- Compiling source code from code.bgl ---" << endl;

    try {
        // 1. Lexical Analysis
        Lexer lexer(sourceCode);

        // 2. Syntax Analysis (Parsing)
        Parser parser(lexer);
        unique_ptr<ProgramNode> ast = parser.parseProgram();
        cout << "Parsing Successful: AST Created." << endl;

        // 3. Code Generation
        CodeGenerator generator;
        string cppCode = generator.generate(ast.get());
        cout << "Code Generation Successful." << endl;

        // 4. Save the generated code to a file
        ofstream outputFile("output.cpp");
        if (outputFile.is_open()) {
            outputFile << cppCode;
            outputFile.close();
            cout << "Successfully saved generated code to output.cpp" << endl;
        } else {
            cerr << "Error: Could not open file output.cpp for writing." << endl;
        }

    } catch (const runtime_error& e) {
        cerr << "Compilation Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
