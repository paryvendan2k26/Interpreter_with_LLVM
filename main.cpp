#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "llvm_codegen.h"

using namespace std;

int main() {

    Interpreter interpreter;

    vector<ASTPtr> trees;

    while (true) {

        string input;

        cout << ">> ";

        getline(cin, input);

        if (
            input == "exit"
        ) {

            break;
        }

        if (input.empty()) {

            continue;
        }

        try {

            Lexer lexer(input);

            vector<Token> tokens =
                lexer.tokenize();

            Parser parser(tokens);

            trees.push_back(
                parser.parse()
            );

            AST* tree =
                trees.back().get();

LLVMCodeGenerator codeGenerator;

codeGenerator.generate(tree);

codeGenerator.printIR();
        }

        catch (
            const exception& error
        ) {

            cerr
                << "Error: "
                << error.what()
                << endl;
        }
    }

    return 0;
}