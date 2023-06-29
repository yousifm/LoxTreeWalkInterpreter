#pragma once

#include <expression_visitor.h>
#include <stmt_visitor.h>
#include <token.h>
#include <interpreter.h>
#include <function_type.h>

#include <deque>
#include <string>
#include <unordered_map>

enum ClassType {
  NONE,
  LoxClass
};

class Resolver : public Expr::ExprVisitor, public Stmt::StmtVisitor {
public:
  Resolver(Interpreter &);

  void visitExprStmt(const Stmt::ExprStmt *) override;
  void visitPrintStmt(const Stmt::PrintStmt *) override;
  void visitVarStmt(const Stmt::VarStmt *) override;
  void visitBlock(const Stmt::Block *) override;
  void visitIfStmt(const Stmt::IfStmt *) override;
  void visitWhileStmt(const Stmt::WhileStmt *) override;
  void visitForStmt(const Stmt::ForStmt *) override;
  void visitFunctionStmt(const Stmt::FunctionStmt *) override;
  void visitReturnStmt(const Stmt::ReturnStmt *) override;
  void visitClassStmt(const Stmt::ClassStmt *) override;

  void visitBinary(const Expr::BinaryExpr *) override;
  void visitLiteral(const Expr::LiteralExpr *) override;
  void visitUnary(const Expr::UnaryExpr *) override;
  void visitGrouping(const Expr::GroupingExpr *) override;
  void visitTernary(const Expr::TernaryExpr *) override;
  void visitVariable(const Expr::VariableExpr *) override;
  void visitAssign(const Expr::AssignExpr *) override;
  void visitLogic(const Expr::LogicExpr *) override;
  void visitCall(const Expr::CallExpr *) override;
  void visitGet(const Expr::GetExpr *) override;
  void visitSet(const Expr::SetExpr *) override;
  void visitThis(const Expr::ThisExpr *) override;

  void resolve(const std::vector<Stmt::Stmt *> &);

private:
  void resolve(const std::vector<const Stmt::Stmt *> &);
  void resolve(const Stmt::Stmt *);
  void resolve(const Expr::Expr *);
  void resolveLocal(const Expr::Expr *, const Token &);
  void resolveFunction(const Stmt::FunctionStmt *, FunctionType);

  void beginScope();
  void endScope();

  void declare(const Token &);
  void define(const Token &);

  Interpreter &_interpreter;
  std::deque<std::unordered_map<std::string, bool>> _scopes;
  
  ClassType currentClass = ClassType::NONE;
};
