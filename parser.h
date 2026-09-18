#pragma once

#include "token.h"
#include "ast.h"
#include <vector>

using namespace std;

class Parser {

public:

    vector<Token> tokens;

    int pos;

    Token currentToken;

    TokenType peekType(int offset = 1);

    Parser(vector<Token> t);

    void advance();

    AST* parse();

    AST* expr();

    AST* term();

    AST* factor();

    AST* statement();

    AST* program();

    AST* ifStatement();

    AST* comparison();

    AST* whileStatement();

    AST* functionDefinition();

AST* functionCall();
};