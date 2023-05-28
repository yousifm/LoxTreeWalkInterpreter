#pragma once

#include "expr.h"
#include <vector>

namespace Stmt {

class StmtVisitor;

class Stmt {
public:
  virtual void accept(StmtVisitor *) const = 0;
};

class ExprStmt : public Stmt {
public:
  explicit ExprStmt(Expr::Expr *);

  void accept(StmtVisitor *) const;
  const Expr::Expr *expr() const { return _expr; }

private:
  const Expr::Expr *_expr;
};

class PrintStmt : public Stmt {
public:
  explicit PrintStmt(Expr::Expr *);

  void accept(StmtVisitor *) const;
  const Expr::Expr *expr() const { return _expr; }

private:
  const Expr::Expr *_expr;
};

class VarStmt : public Stmt {
public:
  explicit VarStmt(Token name, Expr::Expr *init);

  void accept(StmtVisitor *) const;

  const Expr::Expr *init() const { return _init; }
  const Token &name() const { return _name; }

private:
  const Token _name;
  const Expr::Expr *_init;
};

class Block : public Stmt {
public:
  explicit Block(std::vector<const Stmt *> _statements);
  void accept(StmtVisitor *) const;

  const std::vector<const Stmt *> &statements() const { return _statements; }

private:
  const std::vector<const Stmt *> _statements;
};

class IfStmt : public Stmt {
public:
  explicit IfStmt(Expr::Expr *, Stmt *);
  explicit IfStmt(Expr::Expr *, Stmt *, Stmt *);
  void accept(StmtVisitor *) const;

  const Expr::Expr *condition() const { return _condition; }
  const Stmt *thenBranch() const { return _thenBranch; }
  const Stmt *elseBranch() const { return _elseBranch; }

private:
  const Expr::Expr *_condition;
  const Stmt *_thenBranch;
  const Stmt *_elseBranch;
};

} // namespace Stmt
