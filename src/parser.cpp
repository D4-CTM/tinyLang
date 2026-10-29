#include "parser.hpp"
#include "lexer.hpp"
#include <format>
#include <stdexcept>

void Parser::expect(TokenId expected) {
  if (tkn.token != expected) {
    throw std::runtime_error(
        std::format("Expected: {0}. Received: {1}. At [{2}:{3}]",
                    lexer.tokenToString(expected),
                    lexer.tokenToString(tkn.token), tkn.line, tkn.col));
  }

  nextToken();
}

void Parser::program() {
  assign();
  if (tkn.token != TokenId::END_OF_FILE) {
    parse();
  }
}

void Parser::assign() {
  expect(TokenId::IDENTIFIER);
  expect(TokenId::EQUAL);
  expr();
}

void Parser::expr() {
  term();
  while (tkn.token == TokenId::PLUS) {
    nextToken();
    term();
  }
}

void Parser::term() {
  fact();
  while (tkn.token == TokenId::STAR) {
    nextToken();
    fact();
  }
}

void Parser::fact() {
  switch (tkn.token) {
  case TokenId::IDENTIFIER:
  case TokenId::DECIMAL_FLOAT_CONSTANT:
  case TokenId::DECIMAL_INTEGER_CONSTANT:
    nextToken();
    break;

  case TokenId::OPEN_PARENTHESIS:
    nextToken();
    expr();
    expect(TokenId::CLOSE_PARENTHESIS);
    break;

  default:
    throw std::runtime_error(std::format(
R"(Invalid token '{0}' at [{1}:{2}]. Expected:
- NUMERIC VALUE
- IDENTIFIER
- OPEN_PARENTHESIS '(')",
        tkn.data, tkn.line, tkn.col));
  }
}
