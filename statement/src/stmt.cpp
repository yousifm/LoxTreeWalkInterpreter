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

VarStmt::VarStmt(Token name, Expr::Expr *init) : _name(name), _init(init) {}

void VarStmt::accept(StmtVisitor *visitor) const {
  visitor->visitVarStmt(this);
}

} // namespace Stmt
