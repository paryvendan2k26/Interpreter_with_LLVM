#include "llvm_codegen.h"

#include <llvm/Support/raw_ostream.h>

LLVMCodeGenerator::LLVMCodeGenerator()
    :
    context(),
    module(
        std::make_unique<llvm::Module>(
            "MiniLanguage",
            context
        )
    ),
    builder(context)
{
}

void LLVMCodeGenerator::printIR() {

    module->print(
        llvm::outs(),
        nullptr
    );
}