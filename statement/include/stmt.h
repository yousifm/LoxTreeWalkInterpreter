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

class VarStmt : public Stmt {
public:
  VarStmt(Token name, Expr::Expr *init);

  void accept(StmtVisitor *) const;

  const Expr::Expr *init() const { return _init; }
  const Token &name() const { return _name; }

private:
  Token _name;
  Expr::Expr *_init;
};

} // namespace Stmt
