#pragma once

#include "stmt.h"
#include "stmt_visitor.h"
#include <environment.h>
#include <expression_visitor.h>
#include <lox_function.h>

#include <vector>

class Interpreter : public Expr::ExprVisitor, public Stmt::StmtVisitor {
public:
  Interpreter();

  std::any eval(const Expr::Expr *);
  void interpret(const std::vector<Stmt::Stmt *>);

  void visitExprStmt(const Stmt::ExprStmt *) override;
  void visitPrintStmt(const Stmt::PrintStmt *) override;
  void visitVarStmt(const Stmt::VarStmt *) override;
  void visitBlock(const Stmt::Block *) override;
  void visitIfStmt(const Stmt::IfStmt *) override;
  void visitWhileStmt(const Stmt::WhileStmt *) override;
  void visitForStmt(const Stmt::ForStmt *) override;
  void visitFunctionStmt(const Stmt::FunctionStmt *) override;
  void visitReturnStmt(const Stmt::ReturnStmt *) override;

  void visitGrouping(const Expr::GroupingExpr *) override;
  void visitUnary(const Expr::UnaryExpr *) override;
  void visitBinary(const Expr::BinaryExpr *) override;
  void visitLiteral(const Expr::LiteralExpr *) override;
  void visitTernary(const Expr::TernaryExpr *) override;
  void visitVariable(const Expr::VariableExpr *) override;
  void visitAssign(const Expr::AssignExpr *) override;
  void visitLogic(const Expr::LogicExpr *) override;
  void visitCall(const Expr::CallExpr *) override;

  friend class LoxFunction;

private:
  void evalutate(const Expr::Expr *);
  void execute(const Stmt::Stmt *);
  void executeBlock(Environment, const std::vector<const Stmt::Stmt *> &);
  void enforceDouble(Token, const std::any &);
  bool isTruthyExpr(const Expr::Expr *);
  bool isTruthyVal(const std::any &);
  bool isEqual(const std::any &, const std::any &);

  template <typename T> bool isOfType(const std::any &);

  std::any _value;
  Environment _environment;
  Environment globals;
};
