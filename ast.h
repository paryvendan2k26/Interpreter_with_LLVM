#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class AST {
public:

    AST() = default;

    AST(const AST&) = delete;

    AST& operator=(
        const AST&
    ) = delete;

    virtual ~AST() = default;
};

using ASTPtr = unique_ptr<AST>;


class NumberNode : public AST {
public:

    string value;

    NumberNode(string val)
        : value(move(val))
    {
    }
};


class BinaryOpNode : public AST {
public:

    ASTPtr left;
    string op;
    ASTPtr right;

    BinaryOpNode(
        ASTPtr l,
        string o,
        ASTPtr r
    ) :
        left(move(l)),
        op(move(o)),
        right(move(r))
    {
    }
};


class VariableNode : public AST {
public:

    string name;

    VariableNode(string n)
        : name(move(n))
    {
    }
};


class AssignNode : public AST {
public:

    string name;
    ASTPtr value;

    AssignNode(
        string n,
        ASTPtr v
    ) :
        name(move(n)),
        value(move(v))
    {
    }
};


class ProgramNode : public AST {
public:

    vector<ASTPtr> statements;

    ProgramNode(
        vector<ASTPtr> stmts
    ) :
        statements(move(stmts))
    {
    }
};


class IfNode : public AST {
public:

    ASTPtr condition;
    vector<ASTPtr> ifBody;
    vector<ASTPtr> elseBody;

    IfNode(
        ASTPtr cond,
        vector<ASTPtr> ifStmts,
        vector<ASTPtr> elseStmts
    ) :
        condition(move(cond)),
        ifBody(move(ifStmts)),
        elseBody(move(elseStmts))
    {
    }
};


class CompareNode : public AST {
public:

    ASTPtr left;
    string op;
    ASTPtr right;

    CompareNode(
        ASTPtr l,
        string o,
        ASTPtr r
    ) :
        left(move(l)),
        op(move(o)),
        right(move(r))
    {
    }
};


class WhileNode : public AST {
public:

    ASTPtr condition;
    vector<ASTPtr> body;

    WhileNode(
        ASTPtr cond,
        vector<ASTPtr> stmts
    ) :
        condition(move(cond)),
        body(move(stmts))
    {
    }
};


class FunctionDefNode : public AST {
public:

    string name;
    vector<string> params;
    vector<ASTPtr> body;

    FunctionDefNode(
        string n,
        vector<string> p,
        vector<ASTPtr> b
    ) :
        name(move(n)),
        params(move(p)),
        body(move(b))
    {
    }
};


class FunctionCallNode : public AST {
public:

    string name;
    vector<ASTPtr> args;

    FunctionCallNode(
        string n,
        vector<ASTPtr> a
    ) :
        name(move(n)),
        args(move(a))
    {
    }
};