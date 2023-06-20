//
// Created by yousifm on 5/6/23.
//

#include <expr.h>
#include <expression_visitor.h>

namespace Expr {

void BinaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitBinary(this);
}

void UnaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitUnary(this);
}

void LiteralExpr::accept(ExprVisitor *visitor) const {
  visitor->visitLiteral(this);
}

void GroupingExpr::accept(ExprVisitor *visitor) const {
  visitor->visitGrouping(this);
}

void TernaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitTernary(this);
}

void VariableExpr::accept(ExprVisitor* visitor) const {
  visitor->visitVariable(this);
}

void AssignExpr::accept(ExprVisitor* visitor) const {
  visitor->visitAssign(this);
}

void LogicExpr::accept(ExprVisitor* visitor) const {
  visitor->visitLogic(this);
}

void CallExpr::accept(ExprVisitor* visitor) const {
  visitor->visitCall(this);
}

void GetExpr::accept(ExprVisitor* visitor) const {
  visitor->visitGet(this);
}

} // namespace Expr
