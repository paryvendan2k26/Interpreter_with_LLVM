#pragma once

#include <string>

#include <vector>


using namespace std;

class AST {
public:
    virtual ~AST() {}
};

class NumberNode : public AST {
public:
    string value;

    NumberNode(string val) {
        value = val;
    }
};

class BinaryOpNode : public AST {
public:
    AST* left;
    string op;
    AST* right;

    BinaryOpNode(AST* l, string o, AST* r) {
        left = l;
        op = o;
        right = r;
    }

    ~BinaryOpNode() override {

    delete left;
    delete right;
}
};

class VariableNode : public AST {

public:

    string name;

    VariableNode(string n) {
        name = n;
    }
};

class AssignNode : public AST {

public:

    string name;

    AST* value;

    AssignNode(string n, AST* v) {

        name = n;

        value = v;
    }

        ~AssignNode() override {

        delete value;
    }
};


class ProgramNode : public AST {

public:

    vector<AST*> statements;

    ProgramNode(vector<AST*> stmts) {

        statements = stmts;
    }

    ~ProgramNode() override {

    for (AST* statement : statements) {

        delete statement;
    }
}
};

class IfNode : public AST {

public:

    AST* condition;

    vector<AST*> ifBody;

    vector<AST*> elseBody;

    IfNode(
        AST* cond,
        vector<AST*> ifStmts,
        vector<AST*> elseStmts
    ) {

        condition = cond;

        ifBody = ifStmts;

        elseBody = elseStmts;
    }

    ~IfNode() override {

    delete condition;

    for (AST* statement : ifBody) {

        delete statement;
    }

    for (AST* statement : elseBody) {

        delete statement;
    }
}
};


class CompareNode : public AST {

public:

    AST* left;

    string op;

    AST* right;

    CompareNode(
        AST* l,
        string o,
        AST* r
    ) {

        left = l;

        op = o;

        right = r;
    }

    ~CompareNode() override {

    delete left;
    delete right;
}
};

class WhileNode : public AST {

public:

    AST* condition;

    vector<AST*> body;

    WhileNode(
        AST* cond,
        vector<AST*> stmts
    ) {

        condition = cond;

        body = stmts;
    }

    ~WhileNode() override {

    delete condition;

    for (AST* statement : body) {

        delete statement;
    }
}
};

class FunctionDefNode : public AST {

public:

    string name;

    vector<string> params;

    vector<AST*> body;

    FunctionDefNode(
        string n,
        vector<string> p,
        vector<AST*> b
    ) {

        name = n;

        params = p;

        body = b;
    }

    ~FunctionDefNode() override {

    for (AST* statement : body) {

        delete statement;
    }
}
};

class FunctionCallNode : public AST {

public:

    string name;

    vector<AST*> args;

    FunctionCallNode(
        string n,
        vector<AST*> a
    ) {

        name = n;

        args = a;
    }

    ~FunctionCallNode() override {

    for (AST* argument : args) {

        delete argument;
    }
}
};