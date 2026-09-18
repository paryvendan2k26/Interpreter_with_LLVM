#include "lexer.h"
#include <cctype>
#include <stdexcept>

using namespace std;

Lexer::Lexer(string input) {
    text = input;
    pos = 0;

    if (text.empty()) {
        currentChar = '\0';
    } else {
        currentChar = text[pos];
    }
}

void Lexer::advance() {

    pos++;

    if (pos >= text.size()) {

        currentChar = '\0';
    }

    else {

        currentChar = text[pos];
    }
}

vector<Token> Lexer::tokenize() {

    vector<Token> tokens;

    while (currentChar != '\0') {

        if (isdigit(currentChar)) {

            string number = "";

            while (isdigit(currentChar)) {

                number += currentChar;

                advance();
            }

            tokens.push_back(
                Token(NUMBER, number)
            );
        }

        else if (
            isalpha(currentChar) ||
            currentChar == '_'
        ) {

            string name = identifier();

            if (name == "if") {

                tokens.push_back(
                    Token(IF, "if")
                );
            }

            else if (name == "else") {

                tokens.push_back(
                    Token(ELSE, "else")
                );
            }

            else if (name == "while") {

                tokens.push_back(
                    Token(WHILE, "while")
                );
            }

            else if (name == "func") {

                tokens.push_back(
                    Token(FUNC, "func")
                );
            }

            else {

                tokens.push_back(
                    Token(IDENTIFIER, name)
                );
            }
        }

        else if (currentChar == '=') {

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    Token(EQUAL_EQUAL, "==")
                );

                advance();
            }

            else {

                tokens.push_back(
                    Token(EQUAL, "=")
                );
            }
        }

        else if (currentChar == '+') {

            tokens.push_back(
                Token(PLUS, "+")
            );

            advance();
        }

        else if (currentChar == '-') {

            tokens.push_back(
                Token(MINUS, "-")
            );

            advance();
        }

        else if (currentChar == '*') {

            tokens.push_back(
                Token(STAR, "*")
            );

            advance();
        }

        else if (currentChar == '/') {

            tokens.push_back(
                Token(SLASH, "/")
            );

            advance();
        }

        else if (currentChar == '(') {

            tokens.push_back(
                Token(LPAREN, "(")
            );

            advance();
        }

        else if (currentChar == ')') {

            tokens.push_back(
                Token(RPAREN, ")")
            );

            advance();
        }

        else if (currentChar == '{') {

            tokens.push_back(
                Token(LBRACE, "{")
            );

            advance();
        }

        else if (currentChar == '}') {

            tokens.push_back(
                Token(RBRACE, "}")
            );

            advance();
        }

        else if (currentChar == '>') {

            tokens.push_back(
                Token(GREATER, ">")
            );

            advance();
        }

        else if (currentChar == '<') {

            tokens.push_back(
                Token(LESS, "<")
            );

            advance();
        }

        else if (currentChar == ',') {

            tokens.push_back(
                Token(COMMA, ",")
            );

            advance();
        }

        else if (isspace(currentChar)) {

    advance();
}

else {

    throw runtime_error(
        string("Unexpected character: ")
        + currentChar
    );
}
    }

    tokens.push_back(
        Token(EOF_TOKEN, "")
    );

    return tokens;
}

string Lexer::identifier() {

    string result = "";

    while (
        isalnum(currentChar) ||
        currentChar == '_'
    ) {

        result += currentChar;

        advance();
    }

    return result;
}