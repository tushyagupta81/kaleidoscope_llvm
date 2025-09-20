#include "parser.hpp"
#include "AST.hpp"
#include "lexer.hpp"
#include <cstdio>
#include <memory>
#include <vector>

static int CurTok;
static int getNextTok() { return CurTok = gettok(); }

static std::unique_ptr<ExprAST> ParsePrimary();
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPre,
                                              std::unique_ptr<ExprAST> LHS);
static int GetTokPrecedence();

std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(NumVal);
  getNextTok();
  return Result;
}

static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS) {
    return nullptr;
  }

  return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPre,
                                              std::unique_ptr<ExprAST> LHS) {
  while (true) {
    int TokPrec = GetTokPrecedence();

    if (TokPrec < ExprPre) {
      return LHS;
    }

    int BinOp = CurTok;
    getNextTok();

    auto RHS = ParsePrimary();

    if (!RHS) {
      return nullptr;
    }

    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS) {
        return nullptr;
      }
    }
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

static std::unique_ptr<PrototypeAST> ParsePrototype() {
  if (CurTok != tok_identifier) {
    return LogErrorP("Expected function name in prototype");
  }

  std::string FnName = IdentifierStr;
  getNextTok();

  if (CurTok != '(') {
    return LogErrorP("Expected '(' in prototype");
  }

  std::vector<std::string> ArgNames;
  while (getNextTok() == tok_identifier) {
    ArgNames.push_back(IdentifierStr);
  }
  if (CurTok != ')') {
    return LogErrorP("Expected ')' in prototype");
  }

  getNextTok();

  return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

static std::unique_ptr<FunctionAST> ParseDefinition() {
  getNextTok();
  auto Proto = ParsePrototype();
  if (!Proto) {
    return nullptr;
  }

  if (auto E = ParseExpression()) {
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }

  return nullptr;
}

static std::unique_ptr<PrototypeAST> ParseExtern() {
  getNextTok();
  return ParsePrototype();
}

static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }

  return nullptr;
}

static std::unique_ptr<ExprAST> ParseParenExpr() {
  getNextTok();
  auto V = ParseExpression();
  if (!V) {
    return nullptr;
  }

  if (CurTok != ')') {
    return LogError("Expected ')'");
  }

  getNextTok();
  return V;
}

static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;

  getNextTok();

  if (CurTok != '(') {
    return std::make_unique<VariableExprAST>(IdName);
  }

  getNextTok();

  std::vector<std::unique_ptr<ExprAST>> Args;

  if (CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression()) {
        Args.push_back(std::move(Arg));
      } else {
        return nullptr;
      }

      if (CurTok == ')') {
        break;
      }
      if (CurTok != ',') {
        return LogError("Expected ')' or ',' in arguments list");
      }
      getNextTok();
    }
  }

  getNextTok();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

static int GetTokPrecedence() {
  if (!isascii(CurTok)) {
    return -1;
  }
  int TokPre = BinOpPrecedence[CurTok];
  if (TokPre <= 0) {
    return -1;
  }
  return TokPre;
}

static std::unique_ptr<ExprAST> ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("Unknown token when expected expression");
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  }
}

static void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    getNextTok();
  }
}

static void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    getNextTok();
  }
}

static void HandleTopLevelExpression() {
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    getNextTok();
  }
}

void MainLoop() {
  fprintf(stderr, "ready> ");
  getNextTok();
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextTok();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}
