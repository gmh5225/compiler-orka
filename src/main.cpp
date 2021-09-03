//
// Copyright 2021 Patrick Flynn
// This file is part of the Orka compiler.
// Orka is licensed under the BSD-3 license. See the COPYING file for more information.
//
#include <iostream>
#include <string>
#include <cstdio>

#include <preproc/Preproc.hpp>
#include <parser/Parser.hpp>
#include <ast.hpp>

#include <LLVM/Compiler.hpp>
#include <LLIR/LLIRCompiler.hpp>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }
    
    // Compiler (codegen) flags
    CFlags flags;
    flags.name = "a.out";
    flags.nvptx = false;
    
    // Other flags
    std::string input = "";
    bool testLex = false;
    bool printAst = false;
    bool printLLVM = false;
    bool emitLLVM = false;
    bool emitNVPTX = false;
    bool useLLVM = true;
    
    for (int i = 1; i<argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--test-lex") {
            testLex = true;
        } else if (arg == "--ast") {
            printAst = true;
        } else if (arg == "--llvm") {
            printLLVM = true;
        } else if (arg == "--emit-llvm") {
            emitLLVM = true;
        } else if (arg == "--emit-nvptx") {
            emitNVPTX = true;
            flags.nvptx = true;
        } else if (arg == "--host") {
            useLLVM = false;
        } else if (arg == "-o") {
            flags.name = argv[i+1];
            i += 1;
        } else if (arg[0] == '-') {
            std::cerr << "Invalid option: " << arg << std::endl;
            return 1;
        } else {
            input = arg;
        }
    }
    
    std::string newInput = preprocessFile(input);
    if (newInput == "") {
        return 1;
    }
    
    Parser *frontend = new Parser(newInput);
    AstTree *tree;
    
    if (testLex) {
        frontend->debugScanner();
        return 0;
    }
    
    if (!frontend->parse()) {
        delete frontend;
        return 1;
    }
    
    tree = frontend->getTree();
    
    delete frontend;
    remove(newInput.c_str());
    
    if (printAst) {
        tree->print();
        return 0;
    }

    //test
    if (useLLVM) {
        Compiler *compiler = new Compiler(tree, flags);
        compiler->compile();
        
        if (printLLVM) {
            compiler->debug();
            return 0;
        }
        
        if (emitLLVM) {
            std::string output = flags.name;
            if (output == "a.out") {
                output = "./out.ll";
            }
            
            compiler->emitLLVM(output);
            return 0;
        }
        
        compiler->writeAssembly();
        
        if (!emitNVPTX) {
            compiler->assemble();
            compiler->link();
        }
    } else {
        LLIRCompiler *compiler = new LLIRCompiler(tree, "output1");
        compiler->compile();
        compiler->debug();
    }
    
    return 0;
}

