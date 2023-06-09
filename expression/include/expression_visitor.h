#pragma once

#include "expr.h"

namespace Expr {

class ExprVisitor {
public:
  virtual void visitBinary(const BinaryExpr *) = 0;
  virtual void visitLiteral(const LiteralExpr *) = 0;
  virtual void visitUnary(const UnaryExpr *) = 0;
  virtual void visitGrouping(const GroupingExpr *) = 0;
  virtual void visitTernary(const TernaryExpr *) = 0;
  virtual void visitVariable(const VariableExpr *) = 0;
  virtual void visitAssign(const AssignExpr *) = 0;
  virtual void visitLogic(const LogicExpr *) = 0;
  virtual void visitCall(const CallExpr *) = 0;
  virtual void visitGet(const GetExpr *) = 0;
  virtual void visitSet(const SetExpr *) = 0;
  virtual void visitThis(const ThisExpr *) = 0;
};

} // namespace Expr
