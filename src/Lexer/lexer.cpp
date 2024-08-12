#include "tokens.h"

class Lexer;

using RegexHandler = std::function<void(Lexer* lexer, std::regex* regex)>;

class RegexPattern {
    private:
        std::unique_ptr<std::regex> regex;
        RegexHandler handler;

    public:
        // creating a default constructor to set all fields to default values
        RegexPattern() {
            regex = nullptr;
            handler = nullptr;
        }
};

class Lexer {
    public:
        std::vector<Token> Tokens;

        // creating default constructor to set all fields to default values
        Lexer() {
            Source = "";
            Position = 0;
            Tokens = std::vector<Token>();
            Patterns = std::vector<RegexPattern>();
        }

    private:
        std::string Source;
        int Position;
        std::vector<RegexPattern> Patterns;
};

// defining a function that creates a lexer by taking a source string and returning a lexer pointer
Lexer* ConstructLexer(std::string source) {
    Lexer* lexer = new Lexer();
    lexer->Source = source;
    lexer->Position = 0;
    lexer->Tokens = std::vector<Token>();
    lexer->Patterns = {
        
    };
    return lexer;
}

// defining a function called Tokenize which takes a source string and returns an array of tokens
std::vector<Token> Tokenize(std::string source) {

}