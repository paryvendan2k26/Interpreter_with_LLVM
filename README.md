# Interpreter with LLVM

A simple programming language interpreter built from scratch in C++ and gradually being extended with LLVM.

The goal of this project is to understand how programming languages work internally—from reading source code to generating LLVM Intermediate Representation (IR), and eventually executing code through JIT compilation.

## Architecture

```text
Source Code
    ↓
Lexer
    ↓
Tokens
    ↓
Recursive-Descent Parser
    ↓
Abstract Syntax Tree (AST)
    ├── Tree-Walking Interpreter
    └── LLVM IR Generator
```

## Current Features

### Tree-Walking Interpreter

- Integer arithmetic
- Variables and assignments
- Comparison operators
- Parenthesized expressions
- `if` and `else` statements
- `while` loops
- User-defined functions
- Function calls and recursion
- Local and global scopes
- Interactive REPL
- Syntax and runtime error handling

### LLVM Backend

- LLVM 14 integration
- Arithmetic expression code generation
- Signed 32-bit integer values
- Addition, subtraction, multiplication, and division
- Operator precedence and parentheses
- LLVM module and function verification
- Generated LLVM IR output

## Example

Input:

```text
10 + 20 * 3
```

Generated LLVM IR:

```llvm
define i32 @main() {
entry:
  ret i32 70
}
```

## C++ Concepts Used

- Object-oriented programming
- Inheritance and virtual functions
- Recursive-descent parsing
- Recursive AST traversal
- Smart pointers and `std::unique_ptr`
- RAII-based memory management
- Move semantics
- Exception handling
- Hash maps for environments and symbol storage

## Requirements

- C++17-compatible compiler
- LLVM 14
- `llvm-config`

Check the installed LLVM version:

```bash
llvm-config --version
```

## Build and Run

```bash
clang++ *.cpp \
  $(llvm-config --cxxflags) \
  -std=c++17 -fexceptions \
  $(llvm-config --ldflags --system-libs --libs core) \
  -o interpreter

./interpreter
```

## Project Status

This project is under active development. The tree-walking interpreter currently supports the complete language feature set listed above, while the LLVM backend currently supports arithmetic expressions.


## Learning Goals

- Understand how source code is tokenized and parsed
- Learn how an AST represents a program
- Explore how tree-walking interpreters execute code
- Lower AST nodes into LLVM IR
- Use LLVM verification and optimization tools
- Progress from interpretation to JIT compilation

## Repository

[GitHub – Interpreter with LLVM](https://github.com/paryvendan2k26/Interpreter_with_LLVM)

## Author

**Pary Vendan**

Built while learning C++, compiler design, LLVM, and systems programming.
