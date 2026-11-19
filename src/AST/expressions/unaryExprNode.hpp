#pragma once

#include "baseExprNode.hpp"
#include <memory>

class UnaryExprNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::UnaryExpr;
  const std::unique_ptr<BaseExprNode> expr;
  const UnaryOp op;

  explicit UnaryExprNode(SourceLocation loc, UnaryOp op,
                         std::unique_ptr<BaseExprNode> expr)
      : BaseExprNode(kind, loc), op(op), expr(std::move(expr)) {}
};
