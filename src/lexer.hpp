#pragma once

#include "terminals.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

struct Token {
  TokenType type;
  std::string data;
};

#define UNKOWN_TKN                                                             \
  Token{                                                                       \
      TokenType::UNKNOWN,                                                      \
      "",                                                                      \
  };

struct Pos {
  uint32_t line = 1;
  uint32_t col = 1;
  uint32_t pos = 0;

  void movePos();
  void moveLine();

  inline Pos clone() { return Pos{line, col, pos}; }
  inline void dup(Pos &tar) {
    tar.line = line;
    tar.col = col;
    tar.pos = pos;
  }
};

class Lexer {
private:
  Pos pos{0};
  size_t srcLen{0};

  Token getNextToken();
  Token getStringTkn();
  // Returns false only if we reach eof byte.
  bool skipWhitespace();

  // returns raw numeric 0 only if srcLen < pos.pos
  char getNextChar();

public:
  Lexer(std::string path);
  std::string src = "";

  Token peek();
  Token consume();
};
