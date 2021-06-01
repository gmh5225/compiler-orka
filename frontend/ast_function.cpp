#include <iostream>

#include <frontend.hpp>
#include <ast.hpp>

// Builds a function
void Frontend::buildFunction() {
    Token token = scanner->getNext();
    if (token.type != Id) {
        // TODO: Error
        std::cout << "Error: Expected ID" << std::endl;
        return;
    }

    AstFunction *func = new AstFunction(token.id_val);
    tree->addGlobalStatement(func);
    
    // Build the variable body
    token = scanner->getNext();
    while (token.type != Eof && token.type != Begin) {
        switch (token.type) {
            case Id: buildVariableDec(func, token); break;
            
            case Nl: break;
            default: token.print();
        }
        
        token = scanner->getNext();
    }
    
    // Build the body
    token = scanner->getNext();
    while (token.type != Eof && token.type != End) {
        switch (token.type) {
            case Id: {
                Token idToken = token;
                token = scanner->getNext();
                
                if (token.type == Assign) {
                    buildVariableAssign(func, idToken);
                } else if (token.type == LParen) {
                    buildFunctionCallStmt(func, idToken);
                } else {
                    std::cerr << "Invalid ID" << std::endl;
                }
            } break;
            
            case Return: buildReturn(func); break;
            
            case End: 
            case Nl: break;
            default: token.print();
        }
        
        token = scanner->getNext();
    }
}

// Builds an extern function declaration
void Frontend::buildExternFunction() {
    Token token = scanner->getNext();
    if (token.type != Func) {
        std::cerr << "Error: Expected \"func\"." << std::endl;
        return;
    }
    
    token = scanner->getNext();
    if (token.type != Id) {
        std::cerr << "Error: Expected function name." << std::endl;
        return;
    }
    
    std::string funcName = token.id_val;
    std::vector<Var> args;
    
    token = scanner->getNext();
    if (token.type = LParen) {
        token = scanner->getNext();
        while (token.type != Eof && token.type != RParen) {
            Token t1 = token;
            Token t2 = scanner->getNext();
            Token t3 = scanner->getNext();
            Var v;
            
            if (t1.type != Id) {
                std::cerr << "Error: Invalid function argument: Expected name." << std::endl;
                return;
            }
            
            if (t2.type != Colon) {
                std::cerr << "Error: Invalid function argument: Expected \':\'." << std::endl;
                return;
            }
            
            switch (t3.type) {
                case Int: v.type = DataType::Int32; break;
                case Str: {
                    v.type = DataType::Ptr;
                    v.subType = DataType::Char;
                } break;
                
                default: {
                    std::cerr << "Error: Invalid function argument: Unknown type." << std::endl;
                    return;
                }
            }
            
            v.name = t1.id_val;
            args.push_back(v);
            
            token = scanner->getNext();
            if (token.type == Comma) {
                token = scanner->getNext();
            }
        }
    }
    
    AstExternFunction *ex = new AstExternFunction(funcName);
    ex->setArguments(args);
    tree->addGlobalStatement(ex);
}

