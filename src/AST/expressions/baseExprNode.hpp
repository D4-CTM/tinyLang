#pragma once

#include "../baseNode.hpp"

class BaseExprNode : public BaseNode {
protected:
  BaseExprNode(NodeKind kind, SourceLocation loc) : BaseNode(kind, loc) {}
};
