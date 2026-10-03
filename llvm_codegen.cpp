#include "llvm_codegen.h"

#include <stdexcept>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;

LLVMCodeGenerator::LLVMCodeGenerator()
    :
    context(),
    module(
        make_unique<llvm::Module>(
            "MiniLanguage",
            context
        )
    ),
    builder(context)
{
}

llvm::Value*
LLVMCodeGenerator::generateNumber(
    NumberNode* node
) {

    int value =
        stoi(node->value);

    return llvm::ConstantInt::get(
        builder.getInt32Ty(),
        value,
        true
    );
}

llvm::Value*
LLVMCodeGenerator::generateBinaryOp(
    BinaryOpNode* node
) {

    llvm::Value* left =
        generateNode(
            node->left.get()
        );

    llvm::Value* right =
        generateNode(
            node->right.get()
        );

    if (node->op == "+") {

        return builder.CreateAdd(
            left,
            right,
            "addtmp"
        );
    }

    if (node->op == "-") {

        return builder.CreateSub(
            left,
            right,
            "subtmp"
        );
    }

    if (node->op == "*") {

        return builder.CreateMul(
            left,
            right,
            "multmp"
        );
    }

    if (node->op == "/") {

        if (
            auto* constant =
                llvm::dyn_cast<
                    llvm::ConstantInt
                >(right)
        ) {

            if (constant->isZero()) {

                throw runtime_error(
                    "LLVM error: division by zero"
                );
            }
        }

        return builder.CreateSDiv(
            left,
            right,
            "divtmp"
        );
    }

    throw runtime_error(
        "LLVM error: unsupported operator "
        + node->op
    );
}

llvm::Value*
LLVMCodeGenerator::generateProgram(
    ProgramNode* node
) {

    llvm::Value* result = nullptr;

    for (
        const ASTPtr& statement :
        node->statements
    ) {

        result =
            generateNode(
                statement.get()
            );
    }

    if (result == nullptr) {

        throw runtime_error(
            "LLVM error: empty program"
        );
    }

    return result;
}


llvm::Value*
LLVMCodeGenerator::generateNode(
    AST* node
) {

    if (node == nullptr) {

        throw runtime_error(
            "LLVM error: null AST node"
        );
    }

    if (
        auto* number =
            dynamic_cast<NumberNode*>(
                node
            )
    ) {

        return generateNumber(
            number
        );
    }

    if (
        auto* binary =
            dynamic_cast<BinaryOpNode*>(
                node
            )
    ) {

        return generateBinaryOp(
            binary
        );
    }

    if (
        auto* program =
            dynamic_cast<ProgramNode*>(
                node
            )
    ) {

        return generateProgram(
            program
        );
    }

    throw runtime_error(
        "LLVM error: AST node is not supported yet"
    );
}


void LLVMCodeGenerator::generate(
    AST* tree
) {

    llvm::FunctionType* functionType =
        llvm::FunctionType::get(
            builder.getInt32Ty(),
            false
        );

    llvm::Function* function =
        llvm::Function::Create(
            functionType,
            llvm::Function::ExternalLinkage,
            "main",
            module.get()
        );

    llvm::BasicBlock* entryBlock =
        llvm::BasicBlock::Create(
            context,
            "entry",
            function
        );

    builder.SetInsertPoint(
        entryBlock
    );

    llvm::Value* result =
        generateNode(tree);

    builder.CreateRet(result);

    if (
        llvm::verifyFunction(
            *function,
            &llvm::errs()
        )
    ) {

        throw runtime_error(
            "LLVM generated an invalid function"
        );
    }

    if (
        llvm::verifyModule(
            *module,
            &llvm::errs()
        )
    ) {

        throw runtime_error(
            "LLVM generated an invalid module"
        );
    }
}

void LLVMCodeGenerator::printIR() {

    module->print(
        llvm::outs(),
        nullptr
    );
}