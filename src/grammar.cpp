#include "grammar.hpp"
#include "lexer.hpp"
#include <format>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <variant>

void expect(Token tkn, TokenId expected) {
  if (tkn.token != expected) {
    throw std::runtime_error(std::format(
        "Expected: {0}. Received: {1}. At [{2}:{3}]", tokenToString(expected),
        tokenToString(tkn.token), tkn.line, tkn.col));
  }
}

const char *nonTerminalToString(NonTerminals &prod) {
  switch (prod) {
  case NonTerminals::PROGRAM:
    return "PROGRAM";
  case NonTerminals::PROGRAM_P:
    return "PROGRAM_P";
  case NonTerminals::ASSIGN:
    return "ASSIGN";
  case NonTerminals::EXPR:
    return "EXPR";
  case NonTerminals::EXPR_P:
    return "EXPR_P";
  case NonTerminals::TERM:
    return "TERM";
  case NonTerminals::TERM_P:
    return "TERM_P";
  case NonTerminals::FACTOR:
    return "FACTOR";
  }

  std::unreachable();
}

std::uint32_t mapToken(TokenId &tkn) {
  switch (tkn) {
  case TokenId::IDENTIFIER:
    return 0;

  case TokenId::DECIMAL_INTEGER_CONSTANT:
    return 1;

  case TokenId::DECIMAL_FLOAT_CONSTANT:
    return 2;

  case TokenId::EQUAL:
    return 3;

  case TokenId::PLUS:
    return 4;

  case TokenId::STAR:
    return 5;

  case TokenId::OPEN_PARENTHESIS:
    return 6;

  case TokenId::CLOSE_PARENTHESIS:
    return 7;

  case TokenId::SEMICOLON:
    return 8;

  case TokenId::END_OF_FILE:
    return 9;

  default:
    throw std::runtime_error("Unexpected token");
  }
}

Parser::Parser(Lexer &l) : l(l) { buildMatrix(); }

void Parser::buildMatrix() {
  // --- PROGRAM ---
  m.setAt(NonTerminals::PROGRAM, TokenId::IDENTIFIER,
          {NonTerminals::ASSIGN, NonTerminals::PROGRAM_P});

  // --- PROGRAM PRIME ---
  m.setAt(NonTerminals::PROGRAM_P, TokenId::IDENTIFIER,
          {NonTerminals::ASSIGN, NonTerminals::PROGRAM_P});

  m.setAt(NonTerminals::PROGRAM_P, TokenId::END_OF_FILE, {});

  // --- ASSIGN ---
  m.setAt(NonTerminals::ASSIGN, TokenId::IDENTIFIER,
          {TokenId::IDENTIFIER, TokenId::EQUAL, NonTerminals::EXPR,
           TokenId::SEMICOLON});

  // --- EXPRESSION ---
  m.setAt(NonTerminals::EXPR, TokenId::IDENTIFIER,
          {NonTerminals::TERM, NonTerminals::EXPR_P});

  m.setAt(NonTerminals::EXPR, TokenId::DECIMAL_INTEGER_CONSTANT,
          {NonTerminals::TERM, NonTerminals::EXPR_P});

  m.setAt(NonTerminals::EXPR, TokenId::DECIMAL_FLOAT_CONSTANT,
          {NonTerminals::TERM, NonTerminals::EXPR_P});

  m.setAt(NonTerminals::EXPR, TokenId::OPEN_PARENTHESIS,
          {NonTerminals::TERM, NonTerminals::EXPR_P});

  // --- EXPRESSION PRIME ---
  m.setAt(NonTerminals::EXPR_P, TokenId::PLUS,
          {TokenId::PLUS, NonTerminals::TERM, NonTerminals::EXPR_P});

  m.setAt(NonTerminals::EXPR_P, TokenId::CLOSE_PARENTHESIS, {});

  m.setAt(NonTerminals::EXPR_P, TokenId::SEMICOLON, {});

  // --- TERM ---
  m.setAt(NonTerminals::TERM, TokenId::IDENTIFIER,
          {NonTerminals::FACTOR, NonTerminals::TERM_P});

  m.setAt(NonTerminals::TERM, TokenId::DECIMAL_INTEGER_CONSTANT,
          {NonTerminals::FACTOR, NonTerminals::TERM_P});

  m.setAt(NonTerminals::TERM, TokenId::DECIMAL_FLOAT_CONSTANT,
          {NonTerminals::FACTOR, NonTerminals::TERM_P});

  m.setAt(NonTerminals::TERM, TokenId::OPEN_PARENTHESIS,
          {NonTerminals::FACTOR, NonTerminals::TERM_P});

  // --- TERM PRIME ---
  m.setAt(NonTerminals::TERM_P, TokenId::STAR,
          {TokenId::STAR, NonTerminals::FACTOR, NonTerminals::TERM_P});

  m.setAt(NonTerminals::TERM_P, TokenId::PLUS, {});

  m.setAt(NonTerminals::TERM_P, TokenId::CLOSE_PARENTHESIS, {});

  m.setAt(NonTerminals::TERM_P, TokenId::SEMICOLON, {});

  // --- FACTOR ---
  m.setAt(NonTerminals::FACTOR, TokenId::IDENTIFIER, {TokenId::IDENTIFIER});

  m.setAt(NonTerminals::FACTOR, TokenId::DECIMAL_INTEGER_CONSTANT,
          {TokenId::DECIMAL_INTEGER_CONSTANT});

  m.setAt(NonTerminals::FACTOR, TokenId::DECIMAL_FLOAT_CONSTANT,
          {TokenId::DECIMAL_FLOAT_CONSTANT});

  m.setAt(NonTerminals::FACTOR, TokenId::OPEN_PARENTHESIS,
          {TokenId::OPEN_PARENTHESIS, NonTerminals::EXPR,
           TokenId::CLOSE_PARENTHESIS});
}

void printVariant(GrammarItem gi) {
  if (std::holds_alternative<TokenId>(gi)) {
    auto tkn = std::get<TokenId>(gi);

    std::cout << "Token found: " << tokenToString(tkn) << '\n';
  } else if (std::holds_alternative<NonTerminals>(gi)) {
    auto nt = std::get<NonTerminals>(gi);

    std::cout << "NonTerminal found: " << nonTerminalToString(nt) << '\n';
  }
}

void Parser::parse() {
  gs.push(TokenId::END_OF_FILE);
  gs.push(NonTerminals::PROGRAM);

  auto token = l.nextToken();
  while (!gs.empty()) {
    auto x = gs.top();

    if (std::holds_alternative<TokenId>(x)) {
      auto tkn = std::get<TokenId>(x);

      std::cout << "Token found: " << tokenToString(tkn) << '\n';
      std::cout << "Current token: " << tokenToString(token.token) << '\n';
      expect(token, tkn);
      gs.pop();
      token = l.nextToken();
    }

    if (std::holds_alternative<NonTerminals>(x)) {
      auto nt = std::get<NonTerminals>(x);

      std::cout << "NonTerminal found: " << nonTerminalToString(nt) << '\n';
      std::cout << "Current token: " << tokenToString(token.token) << '\n';

      gs.pop();
      auto transition = m.at(nt, token.token);

      if (!transition.has_value()) {
        throw std::runtime_error(
            std::format("Transition from [{0}, {1}] doesn't exist. Crash at: "
                        "[{2}:{3}]",
                        nonTerminalToString(nt), tokenToString(token.token),
                        token.line, token.col));
      }

      auto rules = transition.value();

      for (auto r = rules.rbegin(); r != rules.rend(); ++r) {
        auto gi = *r;

        if (std::holds_alternative<TokenId>(gi)) {
          auto tkn = std::get<TokenId>(gi);

          std::cout << "Token found: " << tokenToString(tkn) << '\n';
        } else if (std::holds_alternative<NonTerminals>(gi)) {
          auto nt = std::get<NonTerminals>(gi);

          std::cout << "NonTerminal found: " << nonTerminalToString(nt) << '\n';
        }

        gs.push(gi);
      }
    }

    std::cout << "Next iter\n\n";
  }
}
