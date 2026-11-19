#pragma once

#include "baseNode.hpp"
#include <memory>
#include <vector>

using ItemsVector = std::vector<std::unique_ptr<BaseNode>>;

class BaseProgramNode : public BaseNode {
public:
    BaseProgramNode(NodeKind kind, SourceLocation loc) 
        : BaseNode(kind, loc) {}
};

class ProgramNode final : public BaseProgramNode {
public:
    static constexpr NodeKind kind = NodeKind::Program;
    const ItemsVector items;

    ProgramNode(SourceLocation loc, ItemsVector items) 
        : BaseProgramNode(kind, loc), items(std::move(items)) {}
};
