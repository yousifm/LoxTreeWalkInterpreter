#pragma once

#include "expr.h"

namespace Stmt {

class StmtVisitor;

class Stmt {
public:
  virtual void accept(StmtVisitor *) const = 0;
};

class ExprStmt : public Stmt {
public:
  ExprStmt(Expr::Expr *);

  void accept(StmtVisitor *) const;
  const Expr::Expr *expr() const { return _expr; }

private:
  Expr::Expr *_expr;
};

class PrintStmt : public Stmt {
public:
  PrintStmt(Expr::Expr *);

  void accept(StmtVisitor *) const;
  const Expr::Expr *expr() const { return _expr; }

private:
  Expr::Expr *_expr;
};

} // namespace Stmt
