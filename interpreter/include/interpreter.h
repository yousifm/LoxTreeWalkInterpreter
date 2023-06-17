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

  LoxType eval(const Expr::Expr *);
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
  
  void resolve(const Expr::Expr*, int);

  friend class LoxFunction;

private:
  void evalutate(const Expr::Expr *);
  void execute(const Stmt::Stmt *);
  void executeBlock(std::shared_ptr<Environment>, const std::vector<const Stmt::Stmt *> &);
  void enforceDouble(Token, const LoxType &);
  bool isTruthyExpr(const Expr::Expr *);
  bool isTruthyVal(const LoxType &);
  LoxType lookupVariable(const Token&, const Expr::Expr*);

  LoxType _value;
  std::shared_ptr<Environment> _globals;
  std::shared_ptr<Environment> _environment;
  std::unordered_map<const Expr::Expr*, int> _locals;
};
