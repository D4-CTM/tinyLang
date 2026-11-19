#pragma once

#include "baseExprNode.hpp"

class FloatLiteralNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::FloatLiteral;
  const float value;

  explicit FloatLiteralNode(SourceLocation loc, float value)
      : BaseExprNode(kind, loc), value(value) {}
};
