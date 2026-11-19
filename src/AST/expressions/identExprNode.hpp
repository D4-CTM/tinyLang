#pragma once

#include "baseExprNode.hpp"
#include <string>

class IdentExprNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::IdentExpr;
  const std::string name;

  explicit IdentExprNode(SourceLocation loc, std::string name)
      : BaseExprNode(kind, loc), name(name) {}
};
