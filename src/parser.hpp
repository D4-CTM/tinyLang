#pragma once
#include "AST/baseNode.hpp"
#include "AST/expressions/baseExprNode.hpp"
#include "AST/programNodes.hpp"
#include "lexer.hpp"
#include <memory>

using BaseNodePtr = std::unique_ptr<BaseNode>;
using ExprNodePtr = std::unique_ptr<BaseExprNode>;
using ProgramNodePtr = std::unique_ptr<BaseProgramNode>;

class Parser {
public:
  Parser(Lexer &lexer) : lexer(lexer), tkn(lexer.nextToken()) {}

  [[nodiscard]]
  inline BaseNodePtr parse() {
    auto p = program();
    expect(TokenId::END_OF_FILE);
    return p;
  }

private:
  inline void nextToken() { tkn = lexer.nextToken(); }

  [[nodiscard]]
  ProgramNodePtr program();

  [[nodiscard]]
  BaseNodePtr assign();

  [[nodiscard]]
  ExprNodePtr expr();

  [[nodiscard]]
  ExprNodePtr term();

  [[nodiscard]]
  ExprNodePtr fact();

  void expect(TokenId expected);

  constexpr SourceLocation loc() { return {tkn.line, tkn.col}; }

  Lexer& lexer;
  Token tkn;
};
