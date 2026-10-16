#pragma once

#include <string>
#include <unordered_map>

enum class TokenType {
  UNKNOWN,

  OPEN_PARENTHESIS,
  CLOSE_PARENTHESIS,

  OPEN_SQUARE_BRACKET,
  CLOSE_SQUARE_BRACKET,

  OPEN_CURLY_BRACKET,
  CLOSE_CURLY_BRACKET,

  SEMICOLON,

  IDENTIFIER,
  DECIMAL_INTEGER_CONSTANT,
  DECIMAL_FLOAT_CONSTANT,
  STRING_CONSTANT,

  CONSTANT_MODIFIER,
  VARIABLE_MODIFIER,

  INTEGER_TYPE,
  FLOAT_TYPE,
  STRING_TYPE,
};

const char *printTokenType(TokenType);

const std::unordered_map<std::string, TokenType> Keywords = {
    {"const", TokenType::CONSTANT_MODIFIER},
    {"var", TokenType::VARIABLE_MODIFIER},

    {"int", TokenType::INTEGER_TYPE},
    {"float", TokenType::FLOAT_TYPE},
    {"string", TokenType::STRING_TYPE}};
