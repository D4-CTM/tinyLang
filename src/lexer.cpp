#include "lexer.hpp"

#include <cctype>
#include <fstream>
#include <iostream>

bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNum(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9') || c == '_' || '.';
}

void Pos::moveLine() {
  line += 1;
  pos += 1;
  col = 0;
}

void Pos::movePos() {
  col += 1;
  pos += 1;
}

Lexer::Lexer(std::string path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Unable to read source code: " << path << '\n';
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    src += line;
    src += '\n';
  }

  srcLen = src.length();
  file.close();
}

char Lexer::getNextChar() {
  if (pos.pos > srcLen)
    return 0;

  char c = src[pos.pos];

  if (c == '\n') {
    pos.moveLine();
    return 0;
  }

  pos.movePos();
  if (isspace(c)) {
    return 0;
  }

  return c;
}

bool Lexer::skipWhitespace() {
  while (pos.pos < srcLen) {
    unsigned char c = src[pos.pos];

    if (c == '\n') {
      pos.moveLine();
      continue;
    }

    if (isspace(c)) {
      pos.movePos();
      continue;
    }

    return true;
  }

  return false;
}

Token Lexer::getStringTkn() {
  Token tkn{TokenType::STRING_CONSTANT, ""};
  while (pos.pos < srcLen) {
    char c = src[pos.pos];

    if (c == '\n') {
      std::cerr << "Unexpected new line while parsing string at (" << pos.line
                << ',' << pos.col << ")\n";
      break;
    }

    pos.movePos();
    if (c == '"') {
      return tkn;
    }

    tkn.data.push_back(c);
  }
  return UNKOWN_TKN;
}

Token Lexer::getNextToken() {
  // current position
  char c = getNextChar();
  switch (c) {
  case '(':
    return Token{TokenType::OPEN_PARENTHESIS, "("};
  case ')':
    return Token{TokenType::CLOSE_PARENTHESIS, ")"};

  case '[':
    return Token{TokenType::OPEN_SQUARE_BRACKET, "["};
  case ']':
    return Token{TokenType::CLOSE_SQUARE_BRACKET, "]"};

  case '{':
    return Token{TokenType::OPEN_CURLY_BRACKET, "{"};
  case '}':
    return Token{TokenType::CLOSE_CURLY_BRACKET, "}"};

  case ';':
    return Token{TokenType::SEMICOLON, ";"};

  case 0:
    return UNKOWN_TKN;
  }

  if (c == '"') {
    return getStringTkn();
  }

  if (!isAlphaNum(c)) {
    std::cerr << "Unexpected token [" << c << "] at (" << pos.line << ','
              << pos.col << ")\n";
    return UNKOWN_TKN;
  }

  Token tkn = Token{};
  tkn.type = isAlpha(c) ? TokenType::IDENTIFIER
             : c == '.' ? TokenType::DECIMAL_FLOAT_CONSTANT
                        : TokenType::DECIMAL_INTEGER_CONSTANT;

  tkn.data.push_back(c);

  Pos oldPos = pos.clone();
  while ((c = getNextChar()) != 0) {
    switch (tkn.type) {
    case TokenType::IDENTIFIER:
      if (!isAlphaNum(c)) {
        break;
      }
      tkn.data.push_back(c);
      pos.dup(oldPos);
      continue;

    case TokenType::DECIMAL_INTEGER_CONSTANT:
      if (c == '.') {
        tkn.type = TokenType::DECIMAL_FLOAT_CONSTANT;
        tkn.data.push_back(c);
        pos.dup(oldPos);
        continue;
      } else if (!isdigit(c)) {
        break;
      }
      tkn.data.push_back(c);
      pos.dup(oldPos);
      continue;

    case TokenType::DECIMAL_FLOAT_CONSTANT:
      if (!isdigit(c)) {
        break;
      }
      tkn.data.push_back(c);
      pos.dup(oldPos);
      continue;

    default:
      break;
    }
  }
  oldPos.dup(pos);

  if (tkn.type == TokenType::DECIMAL_FLOAT_CONSTANT && tkn.data.length() == 1 &&
      tkn.data[0] == '.') {
    std::cerr << "Invalid decimal float constant at: (" << pos.line << ','
              << pos.col << ")\n";
    return UNKOWN_TKN;
  }

  return tkn;
}

Token Lexer::peek() {
  skipWhitespace();
  Pos oldPos = pos.clone();
  Token tkn = getNextToken();
  oldPos.dup(pos);
  return tkn;
}

Token Lexer::consume() {
  skipWhitespace();
  return getNextToken();
}
