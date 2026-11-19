#pragma once

#include "baseExprNode.hpp"

class BoolLiteralNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::BoolLiteral;
  const bool value;

  explicit BoolLiteralNode(SourceLocation loc, bool value)
      : BaseExprNode(kind, loc), value(value) {}
};
