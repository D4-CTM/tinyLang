#pragma once

#include "baseExprNode.hpp"
#include <string>

class StringLiteralNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::StringLiteral;
  const std::string value;

  explicit StringLiteralNode(SourceLocation loc, const char *value)
      : BaseExprNode(kind, loc), value(value) {}

  explicit StringLiteralNode(SourceLocation loc, std::string value)
      : BaseExprNode(kind, loc), value(value) {}
};
