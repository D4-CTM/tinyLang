#include "terminals.hpp"

const char *printTokenType(TokenType tkn) {
  switch (tkn) {
  case TokenType::OPEN_PARENTHESIS:
    return "(";
  case TokenType::CLOSE_PARENTHESIS:
    return ")";

  case TokenType::OPEN_SQUARE_BRACKET:
    return "[";
  case TokenType::CLOSE_SQUARE_BRACKET:
    return "]";

  case TokenType::OPEN_CURLY_BRACKET:
    return "{";
  case TokenType::CLOSE_CURLY_BRACKET:
    return "}";

  case TokenType::SEMICOLON:
    return ";";

  case TokenType::IDENTIFIER:
    return "IDENTIFIER";

  case TokenType::DECIMAL_INTEGER_CONSTANT:
    return "DECIMAL_INTEGER_CONSTANT";
  case TokenType::DECIMAL_FLOAT_CONSTANT:
    return "DECIMAL_FLOAT_CONSTANT";
  case TokenType::STRING_CONSTANT:
    return "STRING_CONSTANT";

  case TokenType::CONSTANT_MODIFIER:
    return "CONSTANT_MODIFIER";
  case TokenType::VARIABLE_MODIFIER:
    return "VARIABLE_MODIFIER";

  case TokenType::INTEGER_TYPE:
    return "INTEGER_TYPE";
  case TokenType::FLOAT_TYPE:
    return "FLOAT_TYPE";
  case TokenType::STRING_TYPE:
    return "STRING_TYPE";

  default:
    return "UNKOWN";
  }
}
