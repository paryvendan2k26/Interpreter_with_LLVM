#include "interpreter.h"
#include <stdexcept>

using namespace std;

int Interpreter::visitNumberNode(NumberNode* node) {

    return stoi(node->value);
}

int Interpreter::visitBinaryOpNode(BinaryOpNode* node) {

    int left =
    visit(node->left.get());

int right =
    visit(node->right.get());

    if (node->op == "+") {
        return left + right;
    }

    else if (node->op == "-") {
        return left - right;
    }

    else if (node->op == "*") {
        return left * right;
    }

    else if (node->op == "/") {

    if (right == 0) {

        throw runtime_error(
            "Division by zero"
        );
    }

    return left / right;
}

    return 0;
}

int Interpreter::visitVariableNode(
    VariableNode* node
) {

    auto variable =
        variables.find(node->name);

    if (variable == variables.end()) {

        throw runtime_error(
            "Undefined variable: "
            + node->name
        );
    }

    return variable->second;
}

int Interpreter::visitAssignNode(
    AssignNode* node
) {

    int value =
    visit(node->value.get());

    variables[node->name] = value;

    return value;
}


int Interpreter::visit(AST* node) {

    NumberNode* numberNode =
        dynamic_cast<NumberNode*>(node);

    if (numberNode != nullptr) {
        return visitNumberNode(numberNode);
    }

    VariableNode* varNode =
        dynamic_cast<VariableNode*>(node);

    if (varNode != nullptr) {
        return visitVariableNode(varNode);
    }

    AssignNode* assignNode =
        dynamic_cast<AssignNode*>(node);

    if (assignNode != nullptr) {
        return visitAssignNode(assignNode);
    }

    ProgramNode* progNode =
    dynamic_cast<ProgramNode*>(node);

    if (progNode != nullptr) {
        return visitProgramNode(progNode);
    }

    BinaryOpNode* binaryNode =
        dynamic_cast<BinaryOpNode*>(node);

    if (binaryNode != nullptr) {
        return visitBinaryOpNode(binaryNode);
    }

    IfNode* ifNode =
    dynamic_cast<IfNode*>(node);

    if (ifNode != nullptr) {
        return visitIfNode(ifNode);
    }

    CompareNode* compareNode =
    dynamic_cast<CompareNode*>(node);

    if (compareNode != nullptr) {
        return visitCompareNode(compareNode);
    }

    WhileNode* whileNode =
    dynamic_cast<WhileNode*>(node);

    if (whileNode != nullptr) {

        return visitWhileNode(
            whileNode
        );
    }

    FunctionDefNode* funcDef =
    dynamic_cast<
        FunctionDefNode*
    >(node);

if (funcDef != nullptr) {

    return visitFunctionDefNode(
        funcDef
    );
}

FunctionCallNode* funcCall =
    dynamic_cast<
        FunctionCallNode*
    >(node);

if (funcCall != nullptr) {

    return visitFunctionCallNode(
        funcCall
    );
}

    return 0;
}

int Interpreter::visitProgramNode(
    ProgramNode* node
) {

    int result = 0;

    for (
    const ASTPtr& stmt :
    node->statements
) {

    result =
        visit(stmt.get());
}

    return result;
}

int Interpreter::visitIfNode(
    IfNode* node
) {

    int cond =  
    visit(node->condition.get());

    int result = 0;

    if (cond) {

        for (
    const ASTPtr& stmt :
    node->ifBody
) {

    result =
        visit(stmt.get());
}
    }

    else {

        for (
    const ASTPtr& stmt :
    node->elseBody
) {

    result =
        visit(stmt.get());
}
    }

    return result;
}

int Interpreter::visitCompareNode(
    CompareNode* node
) {

int left =
    visit(node->left.get());

int right =
    visit(node->right.get());

    if (node->op == ">") {
        return left > right;
    }

    else if (node->op == "<") {
        return left < right;
    }

    else if (node->op == "==") {
        return left == right;
    }

    return 0;
}


int Interpreter::visitWhileNode(
    WhileNode* node
) {

    int result = 0;

    while (
    visit(node->condition.get())
) {

       for (
    const ASTPtr& stmt :
    node->body
) {

    result =
        visit(stmt.get());
}
    }

    return result;
}

int Interpreter::visitFunctionDefNode(
    FunctionDefNode* node
) {

    functions[node->name] = node;

    return 0;
}


int Interpreter::visitFunctionCallNode(
    FunctionCallNode* node
) {

    auto function =
    functions.find(node->name);

if (function == functions.end()) {

    throw runtime_error(
        "Undefined function: "
        + node->name
    );
}

FunctionDefNode* func =
    function->second;

    if (
        node->args.size() !=
        func->params.size()
    ) {

        throw runtime_error(
            "Function '" +
            node->name +
            "' expected " +
            to_string(func->params.size()) +
            " arguments but received " +
            to_string(node->args.size())
        );
    }

    vector<int> argumentValues;

for (
    const ASTPtr& argument :
    node->args
) {

    argumentValues.push_back(
        visit(argument.get())
    );
}

map<string, int> oldVariables =
    variables;

for (
    size_t i = 0;
    i < func->params.size();
    i++
) {

    variables[
        func->params[i]
    ] = argumentValues[i];
}

int result = 0;

try {

    for (
        const ASTPtr& stmt :
        func->body
    ) {

        result =
            visit(stmt.get());
    }
}

catch (...) {

    variables = oldVariables;

    throw;
}

variables = oldVariables;

return result;
}


