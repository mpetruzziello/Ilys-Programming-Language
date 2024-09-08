#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <iomanip>
#include <regex>
#include <string>
#include <initializer_list>
#include <vector>
#include <functional>
#include <memory>
#include <cstddef>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iterator>

enum class TokenType {
    E0F_TOKEN,

    WHITESPACE,
    
    OPENBRACKET,
    CLOSEBRACKET,

    OPENCURLYBRACKET,
    CLOSECURLYBRACKET,

    OPENPARENTHESIS,
    CLOSEPARENTHESIS,

    DOT,
    DOTDOT, // ex: for 0..3 loop (0,1,2,3)

    NUMBER,
    STRING,

    IDENTIFIER,

    ASSIGNMENT, // =
    EQUALS, // ==
    NOT, // !
    NOTEQUALS, // !=

    AND,
    OR,
    COLON,
    SEMICOLON,
    COMMA,
    QUESTIONMARK,
    
    LESSTHAN,
    LESSTHANEQUALS,
    GREATERTHAN,
    GREATERTHANEQUALS,

    //incrementation / decrementation
    PLUSPLUS,
    PLUSEQUALS,
    MINUSMINUS,
    MINUSEQUALS,
    MULTIPLYEQUALS,
    DIVIDEEQUALS,
    MODEQUALS,

    //operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,

    //bool
    TRUE_TOKEN,
    FALSE_TOKEN,

    //kw
    IF,
    ELSE,
    FROM,
    FUNC, // declare functions
    LET,
    CONST,
    TYPEOF,
    NEW,
    IMPORT,
    EXPORT,
    CLASS,
    FOREVERY, // for each loop
    FOR,
    WHILE,
    IN
};

class Token {
    public:
        TokenType type;
        std::string value;

        static std::string GetType(TokenType type) {
            switch (type) {
                case TokenType::E0F_TOKEN: return "EOF";
                case TokenType::OPENBRACKET: return "OPEN BRACKET";
                case TokenType::CLOSEBRACKET: return "CLOSE BRACKET";
                case TokenType::OPENCURLYBRACKET: return "OPEN CURLY BRACKET";
                case TokenType::CLOSECURLYBRACKET: return "CLOSE CURLY BRACKET";
                case TokenType::OPENPARENTHESIS: return "OPEN PARENTHESIS";
                case TokenType::CLOSEPARENTHESIS: return "CLOSE PARENTHESIS";
                case TokenType::DOT: return "DOT";
                case TokenType::DOTDOT: return "DOT DOT";
                case TokenType::NUMBER: return "NUMBER";
                case TokenType::STRING: return "STRING";
                case TokenType::IDENTIFIER: return "IDENTIFIER";
                case TokenType::ASSIGNMENT: return "ASSIGNMENT";
                case TokenType::EQUALS: return "EQUALS";
                case TokenType::NOT: return "NOT";
                case TokenType::NOTEQUALS: return "NOT EQUALS";
                case TokenType::AND: return "AND";
                case TokenType::OR: return "OR";
                case TokenType::COLON: return "COLON";
                case TokenType::SEMICOLON: return "SEMICOLON";
                case TokenType::COMMA: return "COMMA";
                case TokenType::QUESTIONMARK: return "QUESTION MARK";
                case TokenType::LESSTHAN: return "LESS THAN";
                case TokenType::LESSTHANEQUALS: return "LESS THAN OR EQUAL TO";
                case TokenType::GREATERTHAN: return "GREATER THAN";
                case TokenType::GREATERTHANEQUALS: return "GREATER THAN OR EQUAL TO";
                case TokenType::PLUSPLUS: return "PLUS PLUS";
                case TokenType::PLUSEQUALS: return "PLUS EQUALS";
                case TokenType::MINUSMINUS: return "MINUS MINUS";
                case TokenType::MINUSEQUALS: return "MINUS EQUALS";
                case TokenType::MULTIPLYEQUALS: return "MULTIPLY EQUALS";
                case TokenType::DIVIDEEQUALS: return "DIVIDE EQUALS";
                case TokenType::MODEQUALS: return "MOD EQUALS";
                case TokenType::PLUS: return "PLUS";
                case TokenType::MINUS: return "MINUS";
                case TokenType::MULTIPLY: return "MULTIPLY";
                case TokenType::DIVIDE: return "DIVIDE";
                case TokenType::MODULO: return "MODULO";
                case TokenType::IF: return "IF";
                case TokenType::ELSE: return "ELSE";
                case TokenType::FROM: return "FROM";
                case TokenType::FUNC: return "FUNC";
                case TokenType::LET: return "LET";
                case TokenType::CONST: return "CONST";
                case TokenType::TYPEOF: return "TYPEOF";
                case TokenType::NEW: return "NEW";
                case TokenType::IMPORT: return "IMPORT";
                case TokenType::EXPORT: return "EXPORT";
                case TokenType::CLASS: return "CLASS";
                case TokenType::FOREVERY: return "FOR EACH";
                case TokenType::FOR: return "FOR";
                case TokenType::WHILE: return "WHILE";
                case TokenType::IN: return "IN";
                case TokenType::TRUE_TOKEN: return "TRUE";
                case TokenType::FALSE_TOKEN: return "FALSE";
                case TokenType::WHITESPACE: return "WHITESPACE";
                // in case I forget to add a token here, let's put a default case
                default: return "Woopsies, Unknown Token";
            }
        }

        
        static Token ConstructToken(TokenType type, std::string value) {
            Token token;
            token.type = type;
            token.value = value;
            return token;
        }

        static void Debug(const Token& token) {
            if (IsKnown({TokenType::NUMBER, TokenType::STRING, TokenType::IDENTIFIER}, token.type)) {
                std::cout << GetType(token.type) << ": " << token.value << std::endl;
            }
            else {
                std::cout << GetType(token.type) << std::endl;
            }
        }

        static bool IsKnown(std::initializer_list<TokenType> possibilities, TokenType type) {
            for (auto possibility : possibilities) {
                if (possibility == type) {
                    return true;
                }
            }
            return false;
        }
};

#endif