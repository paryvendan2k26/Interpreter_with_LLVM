#pragma once

#include <string>

using namespace std;

enum TokenType {

    NUMBER,
    IDENTIFIER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    EQUAL,
    EQUAL_EQUAL,

    GREATER,
    LESS,

    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    IF,
    ELSE,
    WHILE,
    FUNC,
    COMMA,

    EOF_TOKEN
};

class Token {
public:

    TokenType type;
    string value;

    Token() {
    }

    Token(TokenType t, string v) {
        type = t;
        value = v;
    }

    string identifier();
};