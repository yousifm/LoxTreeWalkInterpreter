#include "stmt_visitor.h"

namespace Stmt {

ExprStmt::ExprStmt(Expr::Expr *expr) : _expr(expr) {}

void ExprStmt::accept(StmtVisitor *visitor) const {
  visitor->visitExprStmt(this);
}

PrintStmt::PrintStmt(Expr::Expr *expr) : _expr(expr) {}

void PrintStmt::accept(StmtVisitor *visitor) const {
  visitor->visitPrintStmt(this);
}

} // namespace Stmt
