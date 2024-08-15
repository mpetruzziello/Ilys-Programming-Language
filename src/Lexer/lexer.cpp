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

// defining a function called "LexAdvance" to advance the lexer's position by a given amount (int)
void LexAdvance(Lexer* lexer, int amount) {
    lexer->Position += amount; // changing the position field of the lexer
}

// defining a function called "TokenPush" to append a token to the lexer's tokens vector
void TokenPush(Lexer* lexer, Token token) {
    lexer->Tokens.push_back(token);
}

// defining a function called "defaultHandler" which takes a type TokenType and a string value and returns a RegexHandler
RegexHandler defaultHandler(TokenType type, std::string value) {
    return [type, value](Lexer* lexer, std::regex* regex) {
        // advancing the lexer's position just past the value we matched and pushing a token
        LexAdvance(lexer, value.length());
        TokenPush(lexer, Token(type, value));
    }
}


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