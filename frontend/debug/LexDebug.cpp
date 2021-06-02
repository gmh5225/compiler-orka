#include <iostream>

#include <lex/Lex.hpp>

void Token::print() {
    switch (type) {
        case EmptyToken: std::cout << "?? "; break;
        case Eof: std::cout << "EOF "; break;
        
        case Extern: std::cout << "EXTERN "; break;
        case Func: std::cout << "FUNC "; break;
        case Begin: std::cout << "BEGIN "; break;
        case End: std::cout << "END "; break;
        case Return: std::cout << "RETURN "; break;
        case Int: std::cout << "INT"; break;
        case Str: std::cout << "STR"; break;
        
        case Id: std::cout << "ID "; break;
        case Int32: std::cout << "I32 "; break;
        
        case Nl: std::cout << "\\n "; break;
        case SemiColon: std::cout << "; "; break;
        case Colon: std::cout << ": "; break;
        case Assign: std::cout << "= "; break;
        case LParen: std::cout << "("; break;
        case RParen: std::cout << ")"; break;
        case Comma: std::cout << ", "; break;
        
        case Plus: std::cout << "+ ";
        case Minus: std::cout << "- ";
        case Mul: std::cout << "* ";
        case Div: std::cout << "/ ";
        
        default: {}
    }
    
    std::cout << id_val << " ";
    std::cout << i32_val << " ";
    
    std::cout << std::endl;
}
