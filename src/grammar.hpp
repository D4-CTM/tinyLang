#pragma once
#include "lexer.hpp"
#include <optional>
#include <stack>
#include <variant>
#include <vector>

enum class NonTerminals {
  PROGRAM,
  PROGRAM_P,
  ASSIGN,
  EXPR,
  EXPR_P,
  TERM,
  TERM_P,
  FACTOR
};

const char *nonTerminalToString(NonTerminals &prod);
std::uint32_t mapToken(TokenId &tkn);

using GrammarItem = std::variant<TokenId, NonTerminals>;

using Productions = std::vector<GrammarItem>;
using GrammarStack = std::stack<GrammarItem>;

template <std::uint32_t rows, std::uint32_t cols> 
struct Matrix {
  std::optional<Productions> data[rows][cols]{};

  void setAt(NonTerminals nonTerm, TokenId tkn, Productions gr) {
    auto r = static_cast<std::uint32_t>(nonTerm);
    auto c = mapToken(tkn);
    data[r][c] = gr;
  };

  std::optional<Productions> operator[](NonTerminals nonTerm, TokenId tkn){
      return at(nonTerm, tkn);
  } 

  std::optional<Productions> at(NonTerminals nonTerm, TokenId tkn) {
    auto r = static_cast<std::uint32_t>(nonTerm);
    auto c = mapToken(tkn);
    return data[r][c];
  };
};

/*
 * Struct made using as a reference:
 * https://www.youtube.com/watch?v=et1fjd8X1ho
 */
template <typename... Callable> struct Visitor : Callable... {
  using Callable::operator()...;
};

class Parser {
public:
  Parser(Lexer &l);

  void parse();
private:
  void buildMatrix();

  Lexer &l;
  Matrix<8, 10> m;
  GrammarStack gs{};
};
