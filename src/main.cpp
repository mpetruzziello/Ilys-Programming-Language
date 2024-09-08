#include "Lexer/lexer.h"

// Creating a function to read testcase1.ilys and compile it
int compileTestcase1() {
    // Opening the binary file
    std::ifstream file("../Test_Cases/testcase1.ilys", std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: testcase1.ilys file" << std::endl;
        return 1;
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Converting the bytes to a string representing the source code
    std::string sourceCode(bytes.begin(), bytes.end());

    // Tokenizing the source code
    std::vector<Token> tokens = Tokenize(sourceCode);

    // Debugging each token in the tokens vector
    for (Token token : tokens) {
        token.Debug(token);
    }

    return 0;
}

// Creating a function to read testcase2.ilys and compile it
int compileTestcase2() {
    // Opening the binary file
    std::ifstream file("../Test_Cases/testcase2.ilys", std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: testcase2.ilys file" << std::endl;
        return 1;
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string sourceCode(bytes.begin(), bytes.end());
    std::vector<Token> tokens = Tokenize(sourceCode);
    for (Token token : tokens) {
        token.Debug(token);
    }
    return 0;
}

// Creating a function to read testcase3.ilys and compile it
int compileTestcase3() {
    // Opening the binary file
    std::ifstream file("../Test_Cases/testcase3.ilys", std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: testcase3.ilys file" << std::endl;
        return 1;
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string sourceCode(bytes.begin(), bytes.end());
    std::vector<Token> tokens = Tokenize(sourceCode);
    for (Token token : tokens) {
        token.Debug(token);
    }
    return 0;
}

int main() {
    compileTestcase1();
    compileTestcase2();
    compileTestcase3();
    return 0;
}