#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

class Lexer;

using RegexHandler = std::function<void(Lexer* lexer, std::regex* regex)>;

class RegexPattern {
    private:
        std::unique_ptr<std::regex> regex;
        RegexHandler handler;

    public:
        RegexPattern();
        RegexPattern(std::unique_ptr<std::regex> regex, RegexHandler handler);
        const std::unique_ptr<std::regex>& GetRegex() const;
        const RegexHandler& GetHandler() const;
};

class Lexer {
    public:
        std::vector<Token> Tokens;
        Lexer();
        int GetPosition() const;
        const std::string& GetSource() const;
        void SetPosition(int position);
        void SetSource(const std::string& source);
        const std::vector<RegexPattern>& GetPatterns() const;
        void SetPatterns(const std::vector<RegexPattern>& patterns);

    private:
        std::string Source;
        int Position;
        std::vector<RegexPattern> Patterns;
};

// defining the functions that will be used in the lexer.cpp file
std::byte SourceAt(Lexer* lexer);

std::string WhatRemains(Lexer* lexer);

bool IsEOF(Lexer* lexer);

void LexAdvance(Lexer* lexer, int amount);

void TokenPush(Lexer* lexer, Token token);

RegexHandler defaultHandler(TokenType type, std::string value);

void numberHandler(Lexer* lexer, std::regex* regex);

Lexer* ConstructLexer(std::string source);

std::vector<Token> Tokenize(std::string source);

#endif