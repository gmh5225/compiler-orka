#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

#include <string>
#include <map>
#include <stack>

#include <ast.hpp>

struct CFlags {
    std::string name;
};

class Compiler {
public:
    explicit Compiler(AstTree *tree, CFlags flags);
    void compile();
    void debug();
    void writeAssembly();
    void assemble();
    void link();
protected:
    void compileStatement(AstStatement *stmt);
    void compileIfStatement(AstStatement *stmt);
    void compileWhileStatement(AstStatement *stmt);
    Value *compileValue(AstExpression *expr);
    Type *translateType(DataType dataType, DataType subType = DataType::Void);
private:
    AstTree *tree;
    CFlags cflags;

    // LLVM stuff
    std::unique_ptr<LLVMContext> context;
    std::unique_ptr<Module> mod;
    std::unique_ptr<IRBuilder<>> builder;
    Function *currentFunc;
    
    // Symbol table
    std::map<std::string, AllocaInst *> symtable;
    std::map<std::string, DataType> typeTable;
    
    // Block stack
    int blockCount = 0;
    std::stack<BasicBlock *> breakStack;
    std::stack<BasicBlock *> continueStack;
};

