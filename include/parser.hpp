#ifndef PARSER_H
#define PARSER_H

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <map>
#include <memory>

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value *> NamedValues;

static std::map<char, int> BinOpPrecedence = {
    {'>', 10},
    {'+', 20},
    {'-', 30},
    {'*', 40},
};

void MainLoop();
llvm::Value *LogErrorV(const char *Str);

#endif // !PARSER_H
