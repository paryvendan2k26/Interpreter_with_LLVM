#include "parser.h"
#include <stdexcept>

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

AST* Parser::factor() {

    if (currentToken.type == NUMBER) {

        AST* node =
            new NumberNode(currentToken.value);

        advance();

        return node;
    }

    else if (
        currentToken.type ==
        IDENTIFIER
    ) {

        if (
            peekType()
            == LPAREN
        ) {

            return functionCall();
        }

        AST* node =
            new VariableNode(
                currentToken.value
            );

        advance();

        return node;
    }

    else if (currentToken.type == LPAREN) {

        advance();

        AST* node = comparison();

       if (currentToken.type != RPAREN) {

    throw runtime_error(
        "Expected ')' after expression"
    );
}

advance();

return node;

        return node;
    }

    return nullptr;
}

AST* Parser::term() {

    AST* node = factor();

    while (
        currentToken.type == STAR ||
        currentToken.type == SLASH
    ) {

        string op = currentToken.value;

        advance();

        AST* right = factor();

        node =
            new BinaryOpNode(node, op, right);
    }

    return node;
}

AST* Parser::expr() {

    AST* node = term();

    while (
        currentToken.type == PLUS ||
        currentToken.type == MINUS
    ) {

        string op = currentToken.value;

        advance();

        AST* right = term();

        node =
            new BinaryOpNode(node, op, right);
    }

    return node;
}

AST* Parser::comparison() {

    AST* node = expr();

    while (
        currentToken.type == GREATER ||
        currentToken.type == LESS ||
        currentToken.type == EQUAL_EQUAL
    ) {

        string op =
            currentToken.value;

        advance();

        AST* right = expr();

        node = new CompareNode(
            node,
            op,
            right
        );
    }

    return node;
}

AST* Parser::ifStatement() {

    advance();

    AST* condition =
        comparison();

    if (currentToken.type == LBRACE) {

        advance();
    }

    vector<AST*> ifBody;

    while (
    currentToken.type != RBRACE &&
    currentToken.type != EOF_TOKEN    ) {

        ifBody.push_back(
            statement()
        );
    }

if (currentToken.type == EOF_TOKEN) {

    throw runtime_error(
        "Expected '}' before end of input"
    );
}

    advance();

    vector<AST*> elseBody;

    if (currentToken.type == ELSE) {

        advance();

        if (currentToken.type == LBRACE) {

            advance();
        }

        while (
        currentToken.type != RBRACE &&
    currentToken.type != EOF_TOKEN        ) {

            elseBody.push_back(
                statement()
            );
        }

            if (currentToken.type == EOF_TOKEN) {

    throw runtime_error(
        "Expected '}' before end of input"
    );

}

        advance();
    }

    return new IfNode(
        condition,
        ifBody,
        elseBody
    );
}
AST* Parser::statement() {

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
        currentToken.type == IDENTIFIER &&
        peekType() == EQUAL
    ) {

        string name = currentToken.value;

        advance();

        advance();

        AST* value =
            comparison();

        return new AssignNode(
            name,
            value
        );
    }

    return comparison();
}

AST* Parser::whileStatement() {

    advance();

    AST* condition =
        comparison();

    if (currentToken.type == LBRACE) {

        advance();
    }

    vector<AST*> body;

    while (
            currentToken.type != RBRACE &&
    currentToken.type != EOF_TOKEN
    ) {

        body.push_back(
            statement()
        );
    }

        if (currentToken.type == EOF_TOKEN) {

    throw runtime_error(
        "Expected '}' before end of input"
    );

}

    advance();

    return new WhileNode(
        condition,
        body
    );
}

AST* Parser::program() {

    vector<AST*> statements;

    while (
        currentToken.type != EOF_TOKEN
    ) {

        statements.push_back(
            statement()
        );
    }

    return new ProgramNode(statements);
}

AST* Parser::functionDefinition() {

    advance();

    string name =
        currentToken.value;

    advance();

    advance();

    vector<string> params;

    while (
        currentToken.type != RPAREN
    ) {

        params.push_back(
            currentToken.value
        );

        advance();

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }
    }

    advance();

    if (
        currentToken.type == LBRACE
    ) {

        advance();
    }

    vector<AST*> body;

    while (
    currentToken.type != RBRACE &&
    currentToken.type != EOF_TOKEN    ) {

        body.push_back(
            statement()
        );
    }

    if (currentToken.type == EOF_TOKEN) {

    throw runtime_error(
        "Expected '}' before end of input"
    );
}

    advance();

    return new FunctionDefNode(
        name,
        params,
        body
    );
}

AST* Parser::functionCall() {

    string name =
        currentToken.value;

    advance();

    advance();

    vector<AST*> args;

    while (
        currentToken.type != RPAREN
    ) {

        args.push_back(
            comparison()
        );

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }
    }

    advance();

    return new FunctionCallNode(
        name,
        args
    );
}


AST* Parser::parse() {

    return program();
}