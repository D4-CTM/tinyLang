#include "printVisitor.hpp"

const char *identifyBinaryOp(BinaryOp op) {
  switch (op) {
  case BinaryOp::OpAdd:
    return "+";
  case BinaryOp::OpSub:
    return "-";
  case BinaryOp::OpMul:
    return "*";
  case BinaryOp::OpDiv:
    return "/";
  default:
    return "Unknown";
  }
}

const char *identifyUnaryOp(UnaryOp op) {
  switch (op) {
  case UnaryOp::OpMinus:
    return "-";
  case UnaryOp::OpNot:
    return "!";
  default:
    return "Unknown";
  }
}
