#ifndef AST_H
#define AST_H


#include <memory>
#include <string>
#include <vector>

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double val);
};

class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(std::string name);
};

class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS);
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args);
};

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(std::string name, std::vector<std::string> args);
    const std::string &getName() const;
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> body);
};

#endif // !AST_H
