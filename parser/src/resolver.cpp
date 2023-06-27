#include <lox.h>
#include <resolver.h>

Resolver::Resolver(Interpreter &interpreter) : _interpreter(interpreter) {}

void Resolver::visitExprStmt(const Stmt::ExprStmt *stmt) {
  resolve(stmt->expr());
}

void Resolver::visitPrintStmt(const Stmt::PrintStmt *stmt) {
  resolve(stmt->expr());
}

void Resolver::visitVarStmt(const Stmt::VarStmt *stmt) {
  declare(stmt->name());
  if (stmt->init() != nullptr) {
    resolve(stmt->init());
  }
  define(stmt->name());
}

void Resolver::visitBlock(const Stmt::Block *block) {
  beginScope();
  resolve(block->statements());
  endScope();
}

void Resolver::visitIfStmt(const Stmt::IfStmt *stmt) {
  resolve(stmt->condition());
  resolve(stmt->thenBranch());
  if (stmt->elseBranch() != nullptr)
    resolve(stmt->elseBranch());
}

void Resolver::visitWhileStmt(const Stmt::WhileStmt *stmt) {
  resolve(stmt->condition());
  resolve(stmt->body());
}

void Resolver::visitForStmt(const Stmt::ForStmt *stmt) {
  resolve(stmt->init());
  resolve(stmt->condition());
  resolve(stmt->body());
  resolve(stmt->after());
}

void Resolver::visitFunctionStmt(const Stmt::FunctionStmt *stmt) {
  declare(stmt->name());
  define(stmt->name());

  resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::visitReturnStmt(const Stmt::ReturnStmt *stmt) {
  if (stmt->expr() != nullptr)
    resolve(stmt->expr());
}

void Resolver::visitClassStmt(const Stmt::ClassStmt *stmt) {
  declare(stmt->name());
  define(stmt->name());

  beginScope();

  _scopes.back()["this"] =  true;

  for (Stmt::FunctionStmt* method : stmt->methods()) {
    resolveFunction(method, FunctionType::METHOD);
  } 

  endScope();
}

void Resolver::visitBinary(const Expr::BinaryExpr *expr) {
  resolve(expr->left());
  resolve(expr->right());
}

void Resolver::visitLiteral(const Expr::LiteralExpr *expr) {}

void Resolver::visitUnary(const Expr::UnaryExpr *expr) {
  resolve(expr->right());
}

void Resolver::visitGrouping(const Expr::GroupingExpr *expr) {
  resolve(expr->expr());
}

void Resolver::visitTernary(const Expr::TernaryExpr *expr) {
  resolve(expr->condition());
  resolve(expr->first());
  resolve(expr->second());
}

void Resolver::visitVariable(const Expr::VariableExpr *expr) {
  if (!_scopes.empty() && _scopes.back().contains(expr->name().lexeme()) &&
      _scopes.back()[expr->name().lexeme()] == false) {
    Lox::runtime_error(RuntimeError(
        expr->name(), "Cannot read local variable in its own initializer."));
  }

  resolveLocal(expr, expr->name());
}

void Resolver::visitAssign(const Expr::AssignExpr *expr) {
  resolve(expr->value());
  resolveLocal(expr, expr->name());
}

void Resolver::visitLogic(const Expr::LogicExpr *expr) {
  resolve(expr->first());
  resolve(expr->second());
}

void Resolver::visitCall(const Expr::CallExpr *expr) {
  resolve(expr->callee());

  for (const Expr::Expr *arg : expr->arguments()) {
    resolve(arg);
  }
}

void Resolver::visitGet(const Expr::GetExpr* expr) {
  resolve(expr->object());
}

void Resolver::visitSet(const Expr::SetExpr* expr) {
  resolve(expr->object());
  resolve(expr->value());
}

void Resolver::visitThis(const Expr::ThisExpr* expr) {
  resolveLocal(expr, expr->keyword());
}

void Resolver::resolve(const std::vector<Stmt::Stmt *> &statements) {
  for (const Stmt::Stmt *stmt : statements) {
    resolve(stmt);
  }
}

void Resolver::resolve(const std::vector<const Stmt::Stmt *> &statements) {
  for (const Stmt::Stmt *stmt : statements) {
    resolve(stmt);
  }
}

void Resolver::resolve(const Stmt::Stmt *stmt) { stmt->accept(this); }

void Resolver::resolve(const Expr::Expr *expression) {
  expression->accept(this);
}

void Resolver::resolveLocal(const Expr::Expr *expr, const Token &name) {
  for (int i = _scopes.size() - 1; i >= 0; i--) {
    if (_scopes[i].contains(name.lexeme())) {
      _interpreter.resolve(expr, _scopes.size() - 1 - i);
      return;
    }
  }
}

void Resolver::resolveFunction(const Stmt::FunctionStmt *stmt, FunctionType type) {
  beginScope();

  for (const Token &param : stmt->params()) {
    declare(param);
    define(param);
  }

  resolve(stmt->body());

  endScope();
}

void Resolver::declare(const Token &name) {
  if (_scopes.empty())
    return;

  _scopes.back()[name.lexeme()] = false;
}

void Resolver::define(const Token &name) {
  if (_scopes.empty())
    return;

  _scopes.back()[name.lexeme()] = true;
}

void Resolver::beginScope() { _scopes.emplace_back(); }

void Resolver::endScope() { _scopes.pop_back(); }
