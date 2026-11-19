#pragma once

#include "baseExprNode.hpp"
#include <memory>

class BinaryExprNode final : public BaseExprNode {
public:
  static constexpr NodeKind kind = NodeKind::BinaryExpr;
  const std::unique_ptr<BaseExprNode> left;
  const std::unique_ptr<BaseExprNode> right;
  const BinaryOp op;

  explicit BinaryExprNode(SourceLocation loc, BinaryOp op,
                         std::unique_ptr<BaseExprNode> left,
                         std::unique_ptr<BaseExprNode> right)
      : BaseExprNode(kind, loc), op(op), left(std::move(left)),
        right(std::move(right)) {}
};
