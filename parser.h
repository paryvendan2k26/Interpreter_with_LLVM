#pragma once

#include "token.h"
#include "ast.h"

#include <cstddef>
#include <vector>

using namespace std;

class Parser {
public:

    vector<Token> tokens;

    size_t pos;

    Token currentToken;

    Parser(vector<Token> t);

    void advance();

    TokenType peekType(
        size_t offset = 1
    );

    ASTPtr parse();

    ASTPtr expr();

    ASTPtr term();

    ASTPtr factor();

    ASTPtr statement();

    ASTPtr program();

    ASTPtr ifStatement();

    ASTPtr comparison();

    ASTPtr whileStatement();

    ASTPtr functionDefinition();

    ASTPtr functionCall();
};