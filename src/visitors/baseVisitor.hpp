#pragma once

#include "../AST/baseNode.hpp"
#include "../AST/programNodes.hpp"
#include "../AST/expressions/intLiteralNode.hpp"
#include "../AST/expressions/boolLiteralNode.hpp"
#include "../AST/expressions/floatLiteralNode.hpp"
#include "../AST/expressions/stringLiteralNode.hpp"
#include "../AST/expressions/callExpr.hpp"
#include "../AST/expressions/identExprNode.hpp"
#include "../AST/expressions/unaryExprNode.hpp"
#include "../AST/expressions/binaryExprNode.hpp"
#include <stdexcept>

template <class R> class Visitor {
public:
  virtual ~Visitor() = default;

  R visit(const BaseNode &n) {
    switch (n.kind()) {
    case NodeKind::IntLiteral:
      return visitIntLiteral(n.as<IntLiteralNode>());
    case NodeKind::FloatLiteral:
      return visitFloatLiteral(n.as<FloatLiteralNode>());
    case NodeKind::BoolLiteral:
      return visitBoolLiteral(n.as<BoolLiteralNode>());
    case NodeKind::StringLiteral:
      return visitStringLiteral(n.as<StringLiteralNode>());
    case NodeKind::IdentExpr:
      return visitIdentExpr(n.as<IdentExprNode>());
    case NodeKind::BinaryExpr:
      return visitBinaryExpr(n.as<BinaryExprNode>());
    case NodeKind::UnaryExpr:
      return visitUnaryExpr(n.as<UnaryExprNode>());
    case NodeKind::CallExpr:
      return visitCallExpr(n.as<CallExprNode>());
    case NodeKind::Program:
      return visitProgram(n.as<ProgramNode>());
    default:
      throw std::logic_error("unknown node kind");
    }
  }

  virtual R visitIntLiteral(const IntLiteralNode &) = 0;
  virtual R visitFloatLiteral(const FloatLiteralNode &) = 0;
  virtual R visitBoolLiteral(const BoolLiteralNode &) = 0;
  virtual R visitStringLiteral(const StringLiteralNode &) = 0;
  virtual R visitIdentExpr(const IdentExprNode &) = 0;
  virtual R visitBinaryExpr(const BinaryExprNode &) = 0;
  virtual R visitUnaryExpr(const UnaryExprNode &) = 0;
  virtual R visitCallExpr(const CallExprNode &) = 0;
  virtual R visitProgram(const ProgramNode &) = 0;
};
