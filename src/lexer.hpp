#pragma once

#include <istream>
#include <string>
#include <unordered_map>

enum class TokenId {
  END_OF_FILE,

  // Single Char Tokens
  OPEN_PARENTHESIS,
  CLOSE_PARENTHESIS,
  OPEN_CURLY_BRACE,
  CLOSE_CURLY_BRACE,
  SEMICOLON,
  EQUAL,

  // MultiChar Tokens
  IDENTIFIER,
  DECIMAL_INTEGER_CONSTANT,
  DECIMAL_FLOAT_CONSTANT,
  SINGLE_LINE_STRING_LITERAL,
  MULTI_LINE_STRING_LITERAL,
  HEX_CONSTANT,
  BINARY_CONSTANT,
  OCTAL_CONSTANT,

  // Keywords
  KW_INT,
  KW_FLOAT,
  KW_WHILE,
  KW_IF,

  // Error Token Identifiers
  INVALID_TOKEN = 255,
  INVALID_DECIMAL_FLOAT_CONSTANT,
  INVALID_HEX_CONSTANT,
  INVALID_BINARY_CONSTANT,
  INVALID_OCTAL_CONSTANT,
  INVALID_FLOAT_CONSTANT,
  INVALID_STRING_LITERAL,
};

const auto keywords = std::unordered_map<std::string, TokenId>({
    {"int", TokenId::KW_INT},
    {"float", TokenId::KW_FLOAT},
    {"while", TokenId::KW_WHILE},
    {"if", TokenId::KW_IF},
});

struct Token {
  TokenId token;
  std::string data;
};

class Lexer {
public:
  Lexer(std::istream &is) : stream(is) { nextChar(); }

  Token nextToken();
  static const char *tokenToString(TokenId tkn);

private:
  inline void nextChar() { ch = stream.get(); }

  std::istream &stream;
  int ch;
};
