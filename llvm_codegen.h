#pragma once

#include "ast.h"

#include <memory>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

class LLVMCodeGenerator {
private:

    llvm::LLVMContext context;

    std::unique_ptr<llvm::Module> module;

    llvm::IRBuilder<> builder;

    llvm::Value* generateNode(
        AST* node
    );

public:

    LLVMCodeGenerator();

    void generate(
        AST* tree
    );

    void printIR();
};