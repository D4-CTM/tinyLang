#pragma once

#include <cstdint>

enum class NodeKind {
  IntLiteral,
  FloatLiteral,
  BoolLiteral,
  StringLiteral,
  IdentExpr,
  UnaryExpr,
  BinaryExpr,
  CallExpr,
  VarDecl,
  Assign,
  CallStmt,
  Print,
  Read,
  If,
  While,
  For,
  Return,
  Block,
  FunctionDecl,
  Param,
  Program
};

enum class BinaryOp {
  OpAdd,
  OpSub,
  OpMul,
  OpDiv,
  OpLt,
  OpLe,
  OpGt,
  OpGe,
  OpEq,
  OpNe,
  OpAnd,
  OpOr,
};

enum class UnaryOp {
  OpNot,
  OpMinus,
};

struct SourceLocation {
  std::uint32_t line;
  std::uint32_t col;
};

class BaseNode {
public:
  virtual ~BaseNode() = default;

  BaseNode() = delete;
  BaseNode(const BaseNode &) = delete;
  BaseNode &operator=(const BaseNode &) = delete;

  NodeKind kind() const { return kind_; }
  SourceLocation loc() const { return loc_; }

  template <class T> bool is() const { return kind_ == T::Kind; }

  template <class T> const T &as() const { return static_cast<const T &>(*this); }

protected:
  explicit BaseNode(NodeKind kind, SourceLocation loc)
      : kind_(kind), loc_(loc) {}

private:
  const NodeKind kind_;
  const SourceLocation loc_;
};
