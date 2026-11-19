#pragma once

#include "baseVisitor.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>

const char *identifyBinaryOp(BinaryOp op);

const char* identifyUnaryOp(UnaryOp op);

class AstPrinter final : public Visitor<void> {
private:
  std::uint8_t depth = 0;
  std::ostream& out;

  std::ostream& pad() {
      out << std::string(depth, ' ');
      return out;
  }

public:
  void visitProgram(const ProgramNode &v) override {
      for (auto& it : v.items) {
          visit(*it);
      }
  }

  AstPrinter(std::ostream& out) : out(out) {} 

  void visitIntLiteral(const IntLiteralNode &v) override {
    pad() << "IntLiteral (" << v.value << ")\n";
  }

  void visitFloatLiteral(const FloatLiteralNode &v) override {
    pad() << "FloatLiteral (" << v.value << ")\n";
  }

  void visitBoolLiteral(const BoolLiteralNode &v) override {
    pad() << "BoolLiteral (" << v.value << ")\n";
  }

  void visitStringLiteral(const StringLiteralNode &v) override {
    pad() << "StringLiteral (" << v.value << ")\n";
  }

  void visitIdentExpr(const IdentExprNode &v) override {
    pad() << "IdentExpr (" << v.name << ")\n";
  }

  void visitBinaryExpr(const BinaryExprNode &v) override {
    pad() << "BinaryExpr (" << identifyBinaryOp(v.op) << ")\n";
    depth += 2;
    visit(*v.left);
    visit(*v.right);
    depth -= 2;
  }

  void visitUnaryExpr(const UnaryExprNode &v) override {
    pad() << "UnaryExpr (" << identifyUnaryOp(v.op) << ")\n";
    depth += 2;
    visit(*v.expr);
    depth -= 2;
  }

  void visitCallExpr(const CallExprNode &v) override {
    pad() << "CallExpr (" << v.name << ")\n";
    depth += 2;
    for (auto& it : v.exprs) {
        visit(*it);
    }
    depth -= 2;
  }
};
