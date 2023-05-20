//
// Created by yousifm on 5/6/23.
//

#include <expr.h>
#include <expression_visitor.h>

namespace Expr {

void BinaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitBinary(shared_from_this());
}

void UnaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitUnary(shared_from_this());
}

void LiteralExpr::accept(ExprVisitor *visitor) const {
  visitor->visitLiteral(shared_from_this());
}

void GroupingExpr::accept(ExprVisitor *visitor) const {
  visitor->visitGrouping(shared_from_this());
}

void TertiaryExpr::accept(ExprVisitor *visitor) const {
  visitor->visitTertiary(shared_from_this());
}

} // namespace Expr
