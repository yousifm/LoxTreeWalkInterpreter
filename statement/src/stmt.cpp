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

Block::Block(std::vector<const Stmt *> statements)
    : _statements(std::move(statements)) {}

void Block::accept(StmtVisitor *visitor) const { visitor->visitBlock(this); }

IfStmt::IfStmt(Expr::Expr *condition, Stmt *thenBranch)
    : _condition(condition), _thenBranch(thenBranch) {}

IfStmt::IfStmt(Expr::Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
    : _condition(condition), _thenBranch(thenBranch), _elseBranch(elseBranch) {}

void IfStmt::accept(StmtVisitor *visitor) const { visitor->visitIfStmt(this); }

WhileStmt::WhileStmt(Expr::Expr *condition, Stmt *body)
    : _condition(condition), _body(body) {}

void WhileStmt::accept(StmtVisitor *visitor) const {
  visitor->visitWhileStmt(this);
}

ForStmt::ForStmt(Stmt *init, Expr::Expr *condition, Expr::Expr *after,
                 Stmt *body)
    : _init(init), _condition(condition), _after(after), _body(body) {}

void ForStmt::accept(StmtVisitor *visitor) const {
  visitor->visitForStmt(this);
}

FunctionStmt::FunctionStmt(Token name, std::vector<Token> &params,
                           std::vector<const Stmt *> &body)
    : _name(name), _params(std::move(params)), _body(std::move(body)) {}

void FunctionStmt::accept(StmtVisitor *visitor) const {
  visitor->visitFunctionStmt(this);
}

} // namespace Stmt
