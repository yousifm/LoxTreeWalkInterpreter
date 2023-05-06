#pragma once

#include "expr.h"

namespace Expr {

class ExprVisitor {
public:
  virtual void visitBinary(std::shared_ptr<const BinaryExpr>) = 0;
  virtual void visitLiteral(std::shared_ptr<const LiteralExpr>) = 0;
  virtual void visitUnary(std::shared_ptr<const UnaryExpr>) = 0;
  virtual void visitGrouping(std::shared_ptr<const GroupingExpr>) = 0;
};

}
