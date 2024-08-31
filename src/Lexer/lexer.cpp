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
        };

        // creating a constructor with parameters
        RegexPattern(std::unique_ptr<std::regex> regex, RegexHandler handler) {
            this->regex = std::move(regex);
            this->handler = handler;
        }

        // Custom copy constructor (deep copy --> cannot copy unique pointers)
        RegexPattern(const RegexPattern& other) {
            if (other.regex) {
                regex = std::make_unique<std::regex>(*other.regex);
            }
            else {
                regex = nullptr;
            }
            handler = other.handler;
        }

        // Custom copy assignment operator (deep copy --> cannot copy unique pointers)
        RegexPattern& operator=(const RegexPattern& other) {
            if (this != &other) {
                if (other.regex) {
                    regex = std::make_unique<std::regex>(*other.regex);
                }
                else {
                    regex = nullptr;
                }
                handler = other.handler;
            }
            return *this;
        }

        // Move constructor and move assignment operator (default implementation)
        RegexPattern(RegexPattern&&) = default;
        RegexPattern& operator=(RegexPattern&&) = default;

        // getter for the regex field
        const std::unique_ptr<std::regex>& GetRegex() const {
            return regex;
        }

        // getter for the handler field
        const RegexHandler& GetHandler() const {
            return handler;
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

        // setter for the position field
        void SetPosition(int position) {
            Position = position;
        }

        // setter for the source field
        void SetSource(const std::string& source) {
            Source = source;
        }

        // getter for the patterns field
        const std::vector<RegexPattern>& GetPatterns() const {
            return Patterns;
        }

        // setter for the patterns field
        void SetPatterns(const std::vector<RegexPattern>& patterns) {
            Patterns = patterns;
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
    if (lexer->GetPosition() < 0 || lexer->GetPosition() >= lexer->GetSource().length()) {
        return "";
    }
    return lexer->GetSource().substr(lexer->GetPosition());
}

// boolean to check if the lexer's position is at an EOF token (greater than or equal to the length of the source string)
bool IsEOF(Lexer* lexer) {
    return lexer->GetPosition() >= lexer->GetSource().length();
}

// defining a function called "LexAdvance" to advance the lexer's position by a given amount (int)
void LexAdvance(Lexer* lexer, int amount) {
    lexer->SetPosition(lexer->GetPosition() + amount);
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
        TokenPush(lexer, Token::ConstructToken(type, value));
    };
}

// defining a function called "skipHandler" which takes a mutable instance of a lexer and a regex pointer and returns nothing
void skipHandler(Lexer* lexer, std::regex* regex) {
    // getting the matched string from the regex
    std::smatch match;
    std::string remains = WhatRemains(lexer);

    // checking if the regex matches the source string at the current position
    if (std::regex_search(remains, match, *regex)) {
        // advancing the lexer's position by the length of the matched string
        LexAdvance(lexer, match.str().length());
    }

    // NOTE: we are not pushing a token here because we are skipping the whitespace
}

// defining a function called "numberHandler" which takes a mutable instance of a lexer and a regex pointer and returns nothing
void numberHandler(Lexer* lexer, std::regex* regex) {
    std::smatch match;
    std::string remains = WhatRemains(lexer);
    if (std::regex_search(remains, match, *regex)) {
        std::string matchedStr = match.str();
        TokenPush(lexer, Token::ConstructToken(TokenType::NUMBER, matchedStr));
        LexAdvance(lexer, matchedStr.length());
    }
}

// defining a function that creates a lexer by taking a source string and returning a lexer pointer
Lexer* ConstructLexer(std::string source) {
    Lexer* lexer = new Lexer();
    lexer->SetSource(source);
    lexer->SetPosition(0);
    lexer->Tokens = std::vector<Token>();
    lexer->SetPatterns({

        // NUMBER (special handler)
        RegexPattern{std::make_unique<std::regex>("-?[0-9]+(\\.[0-9]+)?"), numberHandler},

        // Handling whitespaces (special handler --> skipHandler)
        RegexPattern{std::make_unique<std::regex>("[ \t\n\r]+"), skipHandler},
        
        // OPENBRACKET and CLOSEBRACKET
        RegexPattern{std::make_unique<std::regex>("\\["), defaultHandler(TokenType::OPENBRACKET, "[")},
        RegexPattern{std::make_unique<std::regex>("\\]"), defaultHandler(TokenType::CLOSEBRACKET, "]")},

        // OPENCURLYBRACKET and CLOSECURLYBRACKET
        RegexPattern{std::make_unique<std::regex>("\\{"), defaultHandler(TokenType::OPENCURLYBRACKET, "{")},
        RegexPattern{std::make_unique<std::regex>("\\}"), defaultHandler(TokenType::CLOSECURLYBRACKET, "}")},

        // OPENPARENTHESIS AND CLOSEPARENTHESIS
        RegexPattern{std::make_unique<std::regex>("\\("), defaultHandler(TokenType::OPENPARENTHESIS, "(")},
        RegexPattern{std::make_unique<std::regex>("\\)"), defaultHandler(TokenType::CLOSEPARENTHESIS, ")")},

        // DOT AND DOTDOT
        RegexPattern{std::make_unique<std::regex>("\\."), defaultHandler(TokenType::DOT, ".")},
        RegexPattern{std::make_unique<std::regex>("\\.\\."), defaultHandler(TokenType::DOTDOT, "..")},

        // EQUALS AND NOTEQUALS
        RegexPattern{std::make_unique<std::regex>("=="), defaultHandler(TokenType::EQUALS, "==")},
        RegexPattern{std::make_unique<std::regex>("!="), defaultHandler(TokenType::NOTEQUALS, "!=")},

        // ASSIGNMENT AND NOT
        RegexPattern{std::make_unique<std::regex>("="), defaultHandler(TokenType::ASSIGNMENT, "=")},
        RegexPattern{std::make_unique<std::regex>("!"), defaultHandler(TokenType::NOT, "!")},

        // LESSTHAN AND LESSTHANEQUALS
        RegexPattern{std::make_unique<std::regex>("<="), defaultHandler(TokenType::LESSTHANEQUALS, "<=")},
        RegexPattern{std::make_unique<std::regex>("<"), defaultHandler(TokenType::LESSTHAN, "<")},

        // GREATERTHAN AND GREATERTHANEQUALS
        RegexPattern{std::make_unique<std::regex>(">="), defaultHandler(TokenType::GREATERTHANEQUALS, ">=")},
        RegexPattern{std::make_unique<std::regex>(">"), defaultHandler(TokenType::GREATERTHAN, ">")},

        // AND AND OR
        RegexPattern{std::make_unique<std::regex>("&&"), defaultHandler(TokenType::AND, "&&")},
        RegexPattern{std::make_unique<std::regex>("\\|\\|"), defaultHandler(TokenType::OR, "||")},

        // COLON AND SEMICOLON
        RegexPattern{std::make_unique<std::regex>(":"), defaultHandler(TokenType::COLON, ":")},
        RegexPattern{std::make_unique<std::regex>(";"), defaultHandler(TokenType::SEMICOLON, ";")},

        // QUESTIONMARK AND COMMA
        RegexPattern{std::make_unique<std::regex>("\\?"), defaultHandler(TokenType::QUESTIONMARK, "?")},
        RegexPattern{std::make_unique<std::regex>(","), defaultHandler(TokenType::COMMA, ",")},

        // PLUSPLUS AND MINUSMINUS (CREATING FIRST TO AVOID CONFLICT WITH PLUS AND MINUS DEFINED FIRST)
        RegexPattern{std::make_unique<std::regex>("\\+\\+"), defaultHandler(TokenType::PLUSPLUS, "++")},
        RegexPattern{std::make_unique<std::regex>("--"), defaultHandler(TokenType::MINUSMINUS, "--")},

        // PLUSEQUALS AND MINUSEQUALS
        RegexPattern{std::make_unique<std::regex>("\\+="), defaultHandler(TokenType::PLUSEQUALS, "+=")},
        RegexPattern{std::make_unique<std::regex>("-="), defaultHandler(TokenType::MINUSEQUALS, "-=")},

        // PLUS AND MINUS
        RegexPattern{std::make_unique<std::regex>("\\+"), defaultHandler(TokenType::PLUS, "+")},
        RegexPattern{std::make_unique<std::regex>("-"), defaultHandler(TokenType::MINUS, "-")},

        // DIVIDE AND MULTIPLY
        RegexPattern{std::make_unique<std::regex>("/"), defaultHandler(TokenType::DIVIDE, "/")},
        RegexPattern{std::make_unique<std::regex>("\\*"), defaultHandler(TokenType::MULTIPLY, "*")},

        // MODULO
        RegexPattern{std::make_unique<std::regex>("%"), defaultHandler(TokenType::MODULO, "%")},
    });
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
        for (const RegexPattern& pattern : lexer->GetPatterns()) {
            // checking if the lexer's position is at the end of the source string
            if (IsEOF(lexer)) {
                break;
            }

            // getting the regex pattern from the current pattern
            std::regex* regex = pattern.GetRegex().get();

            // creating a match object to store the results of the regex match
            std::smatch match;

            // checking if the regex pattern matches the source string at the current position
            std::string remains = WhatRemains(lexer);
            if (std::regex_search(remains, match, *regex)) {
                pattern.GetHandler()(lexer, regex);
                matched = true;
                break;
            }
        }

        // what if no pattern was matched?
        if (!matched) {
            // handling the error by throwing an exception
            // not advancing the position by 1 (to avoid infinite loop and for future error handling fixing)
            throw std::runtime_error("No pattern matched at position " + std::to_string(lexer->GetPosition()));

            // LexAdvance(lexer, 1); (comment out but could be used for future)
            // TokenPush(lexer, Token(UNKNOWN, WhatRemains(lexer).substr(0, 1))); (comment out but could be used for future)
            // break; (comment out but could be used for future)
        }
    }

    // Pushing an EOF token to the lexer's tokens vector
    // EOF Token: TokenType::E0F_TOKEN, value: "EOF" --> use ConstructToken
    TokenPush(lexer, Token::ConstructToken(TokenType::E0F_TOKEN, "EOF"));
    return lexer->Tokens;
}