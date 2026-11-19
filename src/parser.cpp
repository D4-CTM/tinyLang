#include "parser.hpp"
#include "AST/baseNode.hpp"
#include "AST/expressions/binaryExprNode.hpp"
#include "AST/expressions/floatLiteralNode.hpp"
#include "AST/expressions/identExprNode.hpp"
#include "AST/expressions/intLiteralNode.hpp"
#include "AST/programNodes.hpp"
#include "lexer.hpp"
#include <format>
#include <memory>
#include <stdexcept>

void Parser::expect(TokenId expected) {
  if (tkn.token != expected) {
    throw std::runtime_error(std::format(
        "Expected: {0}. Received: {1}. At [{2}:{3}]", tokenToString(expected),
        tokenToString(tkn.token), tkn.line, tkn.col));
  }

  nextToken();
}

ProgramNodePtr Parser::program() {
  auto loc_ = loc();
  ItemsVector items;
  items.push_back(std::move(assign()));
  while (tkn.token != TokenId::END_OF_FILE) {
    items.push_back(std::move(assign()));
  }

  return std::make_unique<ProgramNode>(loc_, std::move(items));
}

BaseNodePtr Parser::assign() {
  expect(TokenId::IDENTIFIER);
  expect(TokenId::EQUAL);
  auto ex = expr();
  expect(TokenId::SEMICOLON);
  return ex;
}

ExprNodePtr Parser::expr() {
  SourceLocation loc_ = loc();
  auto lhs = term();
  while (tkn.token == TokenId::PLUS) {
    BinaryOp op = BinaryOp::OpAdd;
    nextToken();
    auto rhs = term();
    lhs = std::make_unique<BinaryExprNode>(loc_, op, std::move(lhs),
                                           std::move(rhs));
  }

  return lhs;
}

ExprNodePtr Parser::term() {
  SourceLocation loc_ = loc();
  auto lhs = fact();
  while (tkn.token == TokenId::STAR) {
    BinaryOp op = BinaryOp::OpMul;
    nextToken();
    auto rhs = fact();
    lhs = std::make_unique<BinaryExprNode>(loc_, op, std::move(lhs),
                                           std::move(rhs));
  }

  return lhs;
}

ExprNodePtr Parser::fact() {
  SourceLocation loc_ = loc();
  switch (tkn.token) {
  case TokenId::IDENTIFIER: {
    auto ident = std::make_unique<IdentExprNode>(loc_, tkn.data);
    nextToken();
    return ident;
  }

  case TokenId::DECIMAL_FLOAT_CONSTANT: {
    auto flt = std::make_unique<FloatLiteralNode>(loc_, std::stof(tkn.data));
    nextToken();
    return flt;
  }

  case TokenId::DECIMAL_INTEGER_CONSTANT: {
    auto intg = std::make_unique<IntLiteralNode>(loc_, std::stoi(tkn.data));
    nextToken();
    return intg;
  }

  case TokenId::OPEN_PARENTHESIS: {
    nextToken();
    auto ex = expr();
    expect(TokenId::CLOSE_PARENTHESIS);
    return ex;
  }

  default:
    throw std::runtime_error(std::format(
        R"(Invalid token '{0}' at [{1}:{2}]. Expected:
- NUMERIC VALUE
- IDENTIFIER
- OPEN_PARENTHESIS '(')",
        tkn.data, tkn.line, tkn.col));
  }
}
