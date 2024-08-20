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

        // getter for the position field
        int GetPosition() const {
            return Position;
        }

        // getter for the source field
        const std::string& GetSource() const {
            return Source;
        }


    private:
        std::string Source;
        int Position;
        std::vector<RegexPattern> Patterns;
};

// returns the byte at the current position of the lexer's source string
std::byte SourceAt(Lexer* lexer) {
    // error checking (returning null byte if the position is out of index bounds)
    if (lexer->GetPosition() < 0 || lexer->GetPosition() >= lexer->GetSource().length()) {
        return static_cast<std::byte>(0);
    }
    return static_cast<std::byte>(lexer->GetSource()[lexer->GetPosition()]);
}

// returns the string from the source at position index to the end of the source string
std::string WhatRemains(Lexer* lexer) {
    if (lexer->GetPosition() < 0 || lexer->Position >= lexer->Source.length()) {
        return "";
    }
    return lexer->Source.substr(lexer->Position);
}

// boolean to check if the lexer's position is at an EOF token (greater than or equal to the length of the source string)
bool IsEOF(Lexer* lexer) {
    return lexer->Position >= lexer->Source.length();
}

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
        // pushing the token created by ConstructToken(TokenType, value)
        TokenPush(lexer, ConstructToken(type, value));
    }
}


// defining a function that creates a lexer by taking a source string and returning a lexer pointer
Lexer* ConstructLexer(std::string source) {
    Lexer* lexer = new Lexer();
    lexer->Source = source;
    lexer->Position = 0;
    lexer->Tokens = std::vector<Token>();
    lexer->Patterns = {
        // OPENBRACKET and CLOSEBRACKET
        RegexPattern{std::make_unique<std::regex>("\\["), defaultHandler(OPENBRACKET, "[")},
        RegexPattern{std::make_unique<std::regex>("\\]"), defaultHandler(CLOSEBRACKET, "]")},

        // OPENCURLYBRACKET and CLOSECURLYBRACKET
        RegexPattern{std::make_unique<std::regex>("\\{"), defaultHandler(OPENCURLYBRACKET, "{")},
        RegexPattern{std::make_unique<std::regex>("\\}"), defaultHandler(CLOSECURLYBRACKET, "}")},

        
        RegexPattern{std::make_unique<std::regex>("\\("), defaultHandler(OPENPARENTHESIS, "(")},
        RegexPattern{std::make_unique<std::regex>("\\)"), defaultHandler(CLOSEPARENTHESIS, ")")},
    };
    return lexer;
}

// defining a function called Tokenize which takes a source string and returns an array of tokens
std::vector<Token> Tokenize(std::string source) {
    // creating a lexer from the source string
    Lexer* lexer = ConstructLexer(source);

    // iterating while the lexer's position is not at the end of the source string (EOF)
    while (!IsEOF(lexer)) {
        
        // declaring variable set to false to check if a pattern was matched
        bool matched = false;

        // iterating through the lexer's patterns
        for (RegexPattern pattern : lexer->Patterns) {
            // checking if the lexer's position is at the end of the source string
            if (IsEOF(lexer)) {
                break;
            }

            // getting the regex pattern from the current pattern
            std::regex* regex = pattern.regex.get();

            // creating a match object to store the results of the regex match
            std::smatch match;

            // checking if the regex pattern matches the source string at the current position
            if (std::regex_search(WhatRemains(lexer), match, *regex)) {
                pattern.handler(lexer, regex);
                matched = true;
                break;
            }
        }

        // what if no pattern was matched?
        if (!matched) {
            // handling the error by throwing an exception
            // not advancing the position by 1 (to avoid infinite loop and for future error handling fixing)
            throw std::runtime_error("No pattern matched at position " + std::to_string(lexer->Position));

            // LexAdvance(lexer, 1); (comment out but could be used for future)
            // TokenPush(lexer, Token(UNKNOWN, WhatRemains(lexer).substr(0, 1))); (comment out but could be used for future)
            // break; (comment out but could be used for future)
        }
    }

    // Pushing an EOF token to the lexer's tokens vector
    // EOF Token: TokenType::E0F_TOKEN, value: "EOF" --> use ConstructToken
    TokenPush(lexer, ConstructToken(E0F_TOKEN, "EOF"));
    return lexer->Tokens;
}