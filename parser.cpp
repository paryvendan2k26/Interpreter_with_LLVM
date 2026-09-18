#include "parser.h"
#include <stdexcept>
#include <utility>
using namespace std;

Parser::Parser(vector<Token> t) {

    tokens = t;

    pos = 0;

    currentToken = tokens[pos];
}

void Parser::advance() {

    pos++;

    if (pos < tokens.size()) {

        currentToken = tokens[pos];
    }
}

TokenType Parser::peekType(int offset) {

    int index = pos + offset;

    if (
        index < 0 ||
        index >= tokens.size()
    ) {
        return EOF_TOKEN;
    }

    return tokens[index].type;
}

ASTPtr Parser::factor() {

    if (currentToken.type == NUMBER) {

        auto node =
            make_unique<NumberNode>(
                currentToken.value
            );

        advance();

        return node;
    }

    else if (
        currentToken.type ==
        IDENTIFIER
    ) {

        if (peekType() == LPAREN) {

            return functionCall();
        }

        auto node =
            make_unique<VariableNode>(
                currentToken.value
            );

        advance();

        return node;
    }

    else if (
        currentToken.type ==
        LPAREN
    ) {

        advance();

        ASTPtr node =
            comparison();

        if (
            currentToken.type !=
            RPAREN
        ) {

            throw runtime_error(
                "Expected ')' after expression"
            );
        }

        advance();

        return node;
    }

    throw runtime_error(
        "Expected a number, variable or '('"
    );
}

ASTPtr Parser::term() {

    ASTPtr node =
        factor();

    while (
        currentToken.type == STAR ||
        currentToken.type == SLASH
    ) {

        string op =
            currentToken.value;

        advance();

        ASTPtr right =
            factor();

        node =
            make_unique<BinaryOpNode>(
                move(node),
                move(op),
                move(right)
            );
    }

    return node;
}

ASTPtr Parser::expr() {

    ASTPtr node =
        term();

    while (
        currentToken.type == PLUS ||
        currentToken.type == MINUS
    ) {

        string op =
            currentToken.value;

        advance();

        ASTPtr right =
            term();

        node =
            make_unique<BinaryOpNode>(
                move(node),
                move(op),
                move(right)
            );
    }

    return node;
}

ASTPtr Parser::comparison() {

    ASTPtr node =
        expr();

    while (
        currentToken.type == GREATER ||
        currentToken.type == LESS ||
        currentToken.type ==
            EQUAL_EQUAL
    ) {

        string op =
            currentToken.value;

        advance();

        ASTPtr right =
            expr();

        node =
            make_unique<CompareNode>(
                move(node),
                move(op),
                move(right)
            );
    }

    return node;
}

ASTPtr Parser::ifStatement() {

    advance();

    ASTPtr condition =
        comparison();

    if (
        currentToken.type !=
        LBRACE
    ) {

        throw runtime_error(
            "Expected '{' after if condition"
        );
    }

    advance();

    vector<ASTPtr> ifBody;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        ifBody.push_back(
            statement()
        );
    }

    if (
        currentToken.type ==
        EOF_TOKEN
    ) {

        throw runtime_error(
            "Expected '}' after if body"
        );
    }

    advance();

    vector<ASTPtr> elseBody;

    if (
        currentToken.type == ELSE
    ) {

        advance();

        if (
            currentToken.type !=
            LBRACE
        ) {

            throw runtime_error(
                "Expected '{' after else"
            );
        }

        advance();

        while (
            currentToken.type != RBRACE &&
            currentToken.type != EOF_TOKEN
        ) {

            elseBody.push_back(
                statement()
            );
        }

        if (
            currentToken.type ==
            EOF_TOKEN
        ) {

            throw runtime_error(
                "Expected '}' after else body"
            );
        }

        advance();
    }

    return make_unique<IfNode>(
        move(condition),
        move(ifBody),
        move(elseBody)
    );
}

ASTPtr Parser::statement() {

    if (currentToken.type == FUNC) {

        return functionDefinition();
    }

    if (currentToken.type == IF) {

        return ifStatement();
    }

    if (currentToken.type == WHILE) {

        return whileStatement();
    }

    if (
        currentToken.type ==
            IDENTIFIER &&
        peekType() == EQUAL
    ) {

        string name =
            currentToken.value;

        advance();
        advance();

        ASTPtr value =
            comparison();

        return make_unique<AssignNode>(
            move(name),
            move(value)
        );
    }

    return comparison();
}

ASTPtr Parser::whileStatement() {

    advance();

    ASTPtr condition =
        comparison();

    if (
        currentToken.type !=
        LBRACE
    ) {

        throw runtime_error(
            "Expected '{' after while condition"
        );
    }

    advance();

    vector<ASTPtr> body;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        body.push_back(
            statement()
        );
    }

    if (
        currentToken.type ==
        EOF_TOKEN
    ) {

        throw runtime_error(
            "Expected '}' after while body"
        );
    }

    advance();

    return make_unique<WhileNode>(
        move(condition),
        move(body)
    );
}

ASTPtr Parser::program() {

    vector<ASTPtr> statements;

    while (
        currentToken.type !=
        EOF_TOKEN
    ) {

        statements.push_back(
            statement()
        );
    }

    return make_unique<ProgramNode>(
        move(statements)
    );
}

ASTPtr Parser::functionDefinition() {

    advance();

    if (
        currentToken.type !=
        IDENTIFIER
    ) {

        throw runtime_error(
            "Expected function name"
        );
    }

    string name =
        currentToken.value;

    advance();

    if (
        currentToken.type !=
        LPAREN
    ) {

        throw runtime_error(
            "Expected '(' after function name"
        );
    }

    advance();

    vector<string> params;

    while (
        currentToken.type != RPAREN &&
        currentToken.type != EOF_TOKEN
    ) {

        if (
            currentToken.type !=
            IDENTIFIER
        ) {

            throw runtime_error(
                "Expected parameter name"
            );
        }

        params.push_back(
            currentToken.value
        );

        advance();

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }

        else if (
            currentToken.type !=
            RPAREN
        ) {

            throw runtime_error(
                "Expected ',' or ')' after parameter"
            );
        }
    }

    if (
        currentToken.type ==
        EOF_TOKEN
    ) {

        throw runtime_error(
            "Expected ')' after parameters"
        );
    }

    advance();

    if (
        currentToken.type !=
        LBRACE
    ) {

        throw runtime_error(
            "Expected '{' before function body"
        );
    }

    advance();

    vector<ASTPtr> body;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        body.push_back(
            statement()
        );
    }

    if (
        currentToken.type ==
        EOF_TOKEN
    ) {

        throw runtime_error(
            "Expected '}' after function body"
        );
    }

    advance();

    return make_unique<FunctionDefNode>(
        move(name),
        move(params),
        move(body)
    );
}


ASTPtr Parser::functionCall() {

    string name =
        currentToken.value;

    advance();

    if (
        currentToken.type !=
        LPAREN
    ) {

        throw runtime_error(
            "Expected '(' after function name"
        );
    }

    advance();

    vector<ASTPtr> args;

    while (
        currentToken.type != RPAREN &&
        currentToken.type != EOF_TOKEN
    ) {

        args.push_back(
            comparison()
        );

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }

        else if (
            currentToken.type !=
            RPAREN
        ) {

            throw runtime_error(
                "Expected ',' or ')' after argument"
            );
        }
    }

    if (
        currentToken.type ==
        EOF_TOKEN
    ) {

        throw runtime_error(
            "Expected ')' after arguments"
        );
    }

    advance();

    return make_unique<FunctionCallNode>(
        move(name),
        move(args)
    );
}

ASTPtr Parser::parse() {

    return program();
}