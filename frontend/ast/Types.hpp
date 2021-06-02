#pragma once

#include <string>

enum class AstType {
    EmptyAst,
    ExternFunc,
    Func,
    Return,
    
    FuncCallStmt,
    FuncCall,
    
    VarDec,
    VarAssign,
    
    Add,
    Sub,
    Mul,
    Div,
    
    IntL,
    StringL,
    ID
};

enum class DataType {
    Void,
    Char,
    Int32,
    Ptr
};

struct Var {
    std::string name;
    DataType type;
    DataType subType;
};
