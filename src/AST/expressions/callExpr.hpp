#pragma once

#include "baseExprNode.hpp"
#include <memory>
#include <vector>

class CallExprNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::CallExpr;
  const std::vector<std::unique_ptr<BaseExprNode>> exprs;
  const std::string name;

  explicit CallExprNode(SourceLocation loc, std::string name,
                         std::vector<std::unique_ptr<BaseExprNode>> exprs)
      : BaseExprNode(kind, loc), name(name), exprs(std::move(exprs)) {}
};
