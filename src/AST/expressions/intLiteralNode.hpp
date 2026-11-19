#pragma once

#include "baseExprNode.hpp"

class IntLiteralNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::IntLiteral;
  const int value;

  explicit IntLiteralNode(SourceLocation loc, int value)
      : BaseExprNode(kind, loc), value(value) {}
};
