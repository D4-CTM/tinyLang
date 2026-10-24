#include "lexer.hpp"

inline bool isHexDigit(char ch) {
  return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') ||
         (ch >= 'A' && ch <= 'F');
}

inline bool isOctDigit(char ch) { return ch >= '0' && ch <= '7'; }

inline bool isBinDigit(char ch) { return ch == '0' || ch == '1'; }

inline bool isDecDigit(char ch) { return (ch >= '0' && ch <= '9'); }

inline bool isAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

inline bool isAlphaNum(char ch) {
  return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ||
         (ch >= 'A' && ch <= 'Z');
}

inline bool isWhitespace(char ch) {
  return (ch >= '\t' && ch <= '\n') || ch == ' ';
}

enum LexerStates {
  INITIAL_STATE,

  // Single Char Tokens
  OPEN_PARENTHESIS_STATE,            // (
  CLOSE_PARENTHESIS_STATE,           // )
  OPEN_CURLY_BRACES_STATE,           // {
  CLOSE_CURLY_BRACES_STATE,          // }
  SEMICOLON_STATE,                   // ;
  EQUAL_STATE,                       // =
                                     //
  NUMERIC_CONSTANT_IDENTIFIER_STATE, // 0
  STARTING_COMMENT_STATE,            // /
  FIRST_STARTING_COMMA_IDENTIFIER,   // "
  SECOND_STARTING_COMMA_IDENTIFIER,  // ""
  FIRST_ENDING_COMMA_IDENTIFIER,     // "
  SECOND_ENDING_COMMA_IDENTIFIER,    // ""

  // Multi-char Single Block states
  IDENTIFIER_STATE,                  // [a-zA-Z][a-zA-Z0-9_]*
  DECIMAL_CONSTANT_DIGIT_STATE,      // [0-9]
  LINE_COMMENT_STATE,                // //
  DECIMAL_FLOAT_CONSTANT_IDENTIFIER, // "." - Intermediate state to separate
                                     // DECIMAL_CONSTANT FROM FLOAT_CONSTAN
  DECIMAL_FLOAT_CONSTANT,            // [0-9]+\.[0-9]+
                                     //
  SINGLE_LINE_STRING_LITERAL,        // \"[^\n\"]\"
  MULTI_LINE_STRING_LITERAL,         // \"\"\"(([^\"]|\"[^\"]|\"\"[^\"]))*\"\"\"
                                     //
                                     //
                                     // 0[xX][0-9a-fA-F]+
  HEXADECIMAL_CONSTANT_STATE_IDENTIFIER, // 0[xX]
  HEXADECIMAL_CONSTANT_STATE,            // [0-9A-Fa-f]+
                                         //
                                         // 0[bB][01]+
  BINARY_CONSTANT_STATE_IDENTIFIER,      // 0[bB]
  BINARY_CONSTANT_STATE,                 // [01]+
                                         //
                                         // 0[oO][0-7]+
  OCTAL_CONSTANT_STATE_IDENTIFIER,       // 0[oO]
  OCTAL_CONSTANT_STATE,                  // [0-7]+

  // Multi-char Multi Block States
  BLOCK_COMMENT_STATE,        // "/*"(.|\n)*"*/"
  BLOCK_COMMENT_FINISH_STATE, // "/"
};

Token Lexer::nextToken() {
  LexerStates state = INITIAL_STATE;
  std::string str = "";
  while (ch != EOF) {
    switch (state) {

    case INITIAL_STATE:
      if (isWhitespace(ch)) {
        nextChar();
        state = INITIAL_STATE;
      } else if (ch == '(') {
        nextChar();
        state = OPEN_PARENTHESIS_STATE;
      } else if (ch == ')') {
        nextChar();
        state = CLOSE_PARENTHESIS_STATE;
      } else if (ch == '{') {
        nextChar();
        state = OPEN_CURLY_BRACES_STATE;
      } else if (ch == '}') {
        nextChar();
        state = CLOSE_CURLY_BRACES_STATE;
      } else if (ch == ';') {
        nextChar();
        state = SEMICOLON_STATE;
      } else if (ch == '=') {
        nextChar();
        state = EQUAL_STATE;
      } else if (isAlpha(ch) || ch == '_') {
        str.push_back(ch);
        nextChar();
        state = IDENTIFIER_STATE;
      } else if (isDecDigit(ch)) {
        str.push_back(ch);

        if (ch == '0') {
          nextChar();
          state = NUMERIC_CONSTANT_IDENTIFIER_STATE;
          continue;
        }

        nextChar();
        state = DECIMAL_CONSTANT_DIGIT_STATE;
      } else if (ch == '/') {
        nextChar();
        state = STARTING_COMMENT_STATE;
      } else if (ch == '"') {
        nextChar();
        state = FIRST_STARTING_COMMA_IDENTIFIER;
      } else {
        str.push_back(ch);
        nextChar();
        return {TokenId::INVALID_TOKEN, str};
      }
      break;

    case OPEN_PARENTHESIS_STATE:
      return {TokenId::OPEN_PARENTHESIS, "("};

    case CLOSE_PARENTHESIS_STATE:
      return {TokenId::CLOSE_PARENTHESIS, ")"};

    case OPEN_CURLY_BRACES_STATE:
      return {TokenId::OPEN_CURLY_BRACE, "{"};

    case CLOSE_CURLY_BRACES_STATE:
      return {TokenId::CLOSE_CURLY_BRACE, "}"};

    case SEMICOLON_STATE:
      return {TokenId::SEMICOLON, ";"};

    case EQUAL_STATE:
      return {TokenId::EQUAL, "="};

    case IDENTIFIER_STATE:
      if (isAlphaNum(ch) || ch == '_') {
        str.push_back(ch);
        nextChar();
      } else {

        if (keywords.contains(str)) {
          return {keywords.at(str), str};
        }

        return {TokenId::IDENTIFIER, str};
      }
      break;

    case DECIMAL_CONSTANT_DIGIT_STATE:
      if (isDecDigit(ch)) {
        str.push_back(ch);
        nextChar();
      } else if (ch == '.') {
        str.push_back(ch);
        nextChar();
        state = DECIMAL_FLOAT_CONSTANT_IDENTIFIER;
      } else {
        return {TokenId::DECIMAL_INTEGER_CONSTANT, str};
      }
      continue;

    case DECIMAL_FLOAT_CONSTANT_IDENTIFIER:
      if (isDecDigit(ch)) {
        str.push_back(ch);
        nextChar();
        state = DECIMAL_FLOAT_CONSTANT;
      } else {
        return {TokenId::INVALID_FLOAT_CONSTANT, str};
      }
      break;

    case DECIMAL_FLOAT_CONSTANT:
      if (isDecDigit(ch)) {
        str.push_back(ch);
        nextChar();
      } else {
        return {TokenId::DECIMAL_FLOAT_CONSTANT, str};
      }
      break;

    // Already read: "
    case FIRST_STARTING_COMMA_IDENTIFIER:
      if (ch == '"') {
        nextChar();
        state = SECOND_STARTING_COMMA_IDENTIFIER;
      } else if (ch == '\n') {
        nextChar();
        return {TokenId::INVALID_STRING_LITERAL, ""};
      } else {
        str.push_back(ch);
        state = SINGLE_LINE_STRING_LITERAL;
      }
      break;

    // Already read: ""
    case SECOND_STARTING_COMMA_IDENTIFIER:
      if (ch == '"') {
        nextChar();
        state = MULTI_LINE_STRING_LITERAL;
      } else {
        nextChar();
        return {TokenId::SINGLE_LINE_STRING_LITERAL, str};
      }
      break;

    case MULTI_LINE_STRING_LITERAL:
      if (ch == '"') {
        nextChar();
        state = FIRST_ENDING_COMMA_IDENTIFIER;
      } else {
        str.push_back(ch);
        nextChar();
      }
      break;

    // Already read: "
    case FIRST_ENDING_COMMA_IDENTIFIER:
      if (ch == '"') {
        nextChar();
        state = SECOND_ENDING_COMMA_IDENTIFIER;
      } else {
        str.push_back('"');
        str.push_back(ch);
        nextChar();

        state = MULTI_LINE_STRING_LITERAL;
      }
      break;

    // Already read: ""
    case SECOND_ENDING_COMMA_IDENTIFIER:
      if (ch == '"') {
        nextChar();
        return {TokenId::MULTI_LINE_STRING_LITERAL, str};
      } else {
        str.push_back('"');
        str.push_back('"');
        str.push_back(ch);
        nextChar();

        state = MULTI_LINE_STRING_LITERAL;
      }
      break;

    case SINGLE_LINE_STRING_LITERAL:
      if (ch == '\"') {
        nextChar();
        return {TokenId::SINGLE_LINE_STRING_LITERAL, str};
      } else if (ch == '\n') {
        nextChar();
        return {TokenId::INVALID_STRING_LITERAL, str};
      }
      str.push_back(ch);
      nextChar();
      break;

    case STARTING_COMMENT_STATE:
      if (ch == '/') {
        nextChar();
        state = LINE_COMMENT_STATE;
      } else if (ch == '*') {
        nextChar();
        state = BLOCK_COMMENT_STATE;
      } else {
        return {TokenId::INVALID_TOKEN, str};
      }
      break;

    case LINE_COMMENT_STATE:
      if (ch == '\n') {
        state = INITIAL_STATE;
      }
      nextChar();
      break;

    case BLOCK_COMMENT_STATE:
      if (ch == '*') {
        state = BLOCK_COMMENT_FINISH_STATE;
      }
      nextChar();
      break;

    case BLOCK_COMMENT_FINISH_STATE:
      if (ch == '/') {
        nextChar();
        state = INITIAL_STATE;
        continue;
      }
      state = BLOCK_COMMENT_STATE;
      break;

    case NUMERIC_CONSTANT_IDENTIFIER_STATE:
      if (ch == 'x' || ch == 'X') {
        str.push_back(ch);
        nextChar();
        state = HEXADECIMAL_CONSTANT_STATE_IDENTIFIER;
      } else if (ch == 'b' || ch == 'B') {
        str.push_back(ch);
        nextChar();
        state = BINARY_CONSTANT_STATE_IDENTIFIER;
      } else if (ch == 'o' || ch == 'O') {
        str.push_back(ch);
        nextChar();
        state = OCTAL_CONSTANT_STATE_IDENTIFIER;
      } else {
        state = DECIMAL_CONSTANT_DIGIT_STATE;
      }
      break;

    case HEXADECIMAL_CONSTANT_STATE_IDENTIFIER:
      if (isHexDigit(ch)) {
        str.push_back(ch);
        nextChar();
        state = HEXADECIMAL_CONSTANT_STATE;
      } else {
        return {TokenId::INVALID_HEX_CONSTANT, str};
      }
      break;

    case HEXADECIMAL_CONSTANT_STATE:
      if (isHexDigit(ch)) {
        str.push_back(ch);
        nextChar();
      } else {
        return {TokenId::HEX_CONSTANT, str};
      }
      break;

    case BINARY_CONSTANT_STATE_IDENTIFIER:
      if (isBinDigit(ch)) {
        str.push_back(ch);
        nextChar();
        state = BINARY_CONSTANT_STATE;
      } else {
        return {TokenId::INVALID_BINARY_CONSTANT, str};
      }
      break;

    case BINARY_CONSTANT_STATE:
      if (isBinDigit(ch)) {
        str.push_back(ch);
        nextChar();
      } else {
        return {TokenId::BINARY_CONSTANT, str};
      }
      break;

    case OCTAL_CONSTANT_STATE_IDENTIFIER:
      if (isOctDigit(ch)) {
        str.push_back(ch);
        nextChar();
        state = OCTAL_CONSTANT_STATE;
      } else {
        return {TokenId::INVALID_OCTAL_CONSTANT, str};
      }
      break;

    case OCTAL_CONSTANT_STATE:
      if (isOctDigit(ch)) {
        str.push_back(ch);
        nextChar();
      } else {
        return {TokenId::OCTAL_CONSTANT, str};
      }
      break;

    default:
      nextChar();
      return {TokenId::INVALID_TOKEN, str};
    }
  }

  return {TokenId::END_OF_FILE, "<<EOF>>"};
}

const char *Lexer::tokenToString(TokenId tkn) {
  switch (tkn) {
  case TokenId::OPEN_PARENTHESIS:
    return "OPEN_PARENTHESIS";
  case TokenId::CLOSE_PARENTHESIS:
    return "CLOSE_PARENTHESIS";
  case TokenId::OPEN_CURLY_BRACE:
    return "OPEN_CURLY_BRACE";
  case TokenId::CLOSE_CURLY_BRACE:
    return "CLOSE_CURLY_BRACE";
  case TokenId::SEMICOLON:
    return "SEMICOLON";
  case TokenId::IDENTIFIER:
    return "IDENTIFIER";
  case TokenId::DECIMAL_INTEGER_CONSTANT:
    return "DECIMAL_INTEGER_CONSTANT";
  case TokenId::DECIMAL_FLOAT_CONSTANT:
    return "DECIMAL_FLOAT_CONSTANT";
  case TokenId::SINGLE_LINE_STRING_LITERAL:
    return "SINGLE_LINE_STRING_LITERAL";
  case TokenId::MULTI_LINE_STRING_LITERAL:
    return "MULTI_LINE_STRING_LITERAL";
  case TokenId::HEX_CONSTANT:
    return "HEX_CONSTANT";
  case TokenId::BINARY_CONSTANT:
    return "BINARY_CONSTANT";
  case TokenId::EQUAL:
    return "EQUAL";
  case TokenId::OCTAL_CONSTANT:
    return "OCTAL_CONSTANT";
  case TokenId::INVALID_TOKEN:
    return "INVALID_TOKEN";
  case TokenId::KW_INT:
    return "KW_INT";
  case TokenId::KW_FLOAT:
    return "KW_FLOAT";
  case TokenId::KW_WHILE:
    return "KW_WHILE";
  case TokenId::KW_IF:
    return "KW_IF";
  case TokenId::INVALID_DECIMAL_FLOAT_CONSTANT:
    return "INVALID_DECIMAL_FLOAT_CONSTANT";
  case TokenId::INVALID_HEX_CONSTANT:
    return "INVALID_HEX_CONSTANT";
  case TokenId::INVALID_BINARY_CONSTANT:
    return "INVALID_BINARY_CONSTANT";
  case TokenId::INVALID_OCTAL_CONSTANT:
    return "INVALID_OCTAL_CONSTANT";
  case TokenId::INVALID_FLOAT_CONSTANT:
    return "INVALID_FLOAT_CONSTANT";
  case TokenId::INVALID_STRING_LITERAL:
    return "INVALID_STRING_LITERAL";

  case TokenId::END_OF_FILE:
    return "END_OF_FILE";

  default:
    return "<<UNMAPPED TOKEN>>";
  }
}
