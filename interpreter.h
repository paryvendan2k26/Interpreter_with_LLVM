#pragma once

#include "ast.h"

#include <map>

using namespace std;

class Interpreter {

        map<string, int> variables;
        map<string, FunctionDefNode*> functions;

public:

    int visit(AST* node);

    int visitNumberNode(NumberNode* node);

    int visitBinaryOpNode(BinaryOpNode* node);

    int visitVariableNode(VariableNode* node);

    int visitAssignNode(AssignNode* node);

    int visitProgramNode(ProgramNode* node);

    int visitIfNode(IfNode* node);

    int visitCompareNode(
    CompareNode* node
);

    int visitWhileNode(
        WhileNode* node
    );

    int visitFunctionDefNode(
    FunctionDefNode* node
);

int visitFunctionCallNode(
    FunctionCallNode* node
);
    

};