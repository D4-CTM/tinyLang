#pragma once
#include "lexer.hpp"

class Parser {
public:
  Parser(std::istream &is) : lexer(is), tkn(lexer.nextToken()) {}

  inline void parse() {
      program();
      expect(TokenId::END_OF_FILE);
  }

private:
  inline void nextToken() { tkn = lexer.nextToken(); }

  void program();
  void assign();
  void expr();
  void term();
  void fact();

  void expect(TokenId expected);

  Lexer lexer;
  Token tkn;
};
