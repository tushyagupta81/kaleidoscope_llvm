#include "AST.hpp"
#include <utility>

NumberExprAST::NumberExprAST(double val) : Val(val) {}

VariableExprAST::VariableExprAST(std::string name) : Name(std::move(name)) {}

BinaryExprAST::BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

CallExprAST::CallExprAST(std::string callee,
                         std::vector<std::unique_ptr<ExprAST>> args)
    : Callee(std::move(callee)), Args(std::move(args)) {}

PrototypeAST::PrototypeAST(std::string name, std::vector<std::string> args)
    : Name(std::move(name)), Args(std::move(args)) {}

const std::string &PrototypeAST::getName() const { return Name; }

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> body)
    : Proto(std::move(Proto)), Body(std::move(body)) {}
