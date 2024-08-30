#include "lexer.h"


int main() {

    // Opening the binary file
    std::ifstream file("../Test_Cases/cases.ilys", std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: cases.ilys file" << std::endl;
        return 1;
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Converting the bytes to a string representing the source code
    std::string sourceCode(bytes.begin(), bytes.end());
}