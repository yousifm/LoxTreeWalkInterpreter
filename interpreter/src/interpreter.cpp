#include "interpreter.h"
#include "lox.h"
#include "lox_callable.h"
#include "native_func.h"
#include "runtime_error.h"
#include "token_type.h"

#include <any>
#include <sstream>

Interpreter::Interpreter() {
  globals.define("clock", (std::any)Clock());
  _environment = globals;
}

void Interpreter::interpret(const std::vector<Stmt::Stmt *> statements) {
  try {
    for (const Stmt::Stmt *statement : statements) {
      statement->accept(this);
    }
  } catch (RuntimeError err) {
    Lox::runtime_error(err);
  }
}

std::any Interpreter::eval(const Expr::Expr *expr) {
  evalutate(expr);
  return _value;
}

void Interpreter::visitExprStmt(const Stmt::ExprStmt *stmt) {
  evalutate(stmt->expr());
}

void Interpreter::visitPrintStmt(const Stmt::PrintStmt *stmt) {
  std::any val = eval(stmt->expr());
  Lox::print_any(val);
}

void Interpreter::visitVarStmt(const Stmt::VarStmt *stmt) {
  std::any val = nullptr;

  if (stmt->init() != nullptr) {
    val = eval(stmt->init());
  }

  _environment.define(stmt->name().lexeme(), val);
}

void Interpreter::visitBlock(const Stmt::Block *block) {
  executeBlock(_environment, block->statements());
}

void Interpreter::visitIfStmt(const Stmt::IfStmt *stmt) {
  if (isTruthyExpr(stmt->condition())) {
    execute(stmt->thenBranch());
  } else {
    execute(stmt->elseBranch());
  }
}

void Interpreter::visitWhileStmt(const Stmt::WhileStmt *stmt) {
  while (isTruthyExpr(stmt->condition())) {
    execute(stmt->body());
  }
}

void Interpreter::visitForStmt(const Stmt::ForStmt *stmt) {
  if (stmt->init() != nullptr)
    execute(stmt->init());
  while (stmt->condition() == nullptr || isTruthyExpr(stmt->condition())) {
    execute(stmt->body());
    if (stmt->after() != nullptr)
      evalutate(stmt->after());
  }
}

void Interpreter::visitFunctionStmt(const Stmt::FunctionStmt *stmt) {
  LoxFunction function = LoxFunction(*stmt);
  _environment.define(stmt->name().lexeme(), function);
}

void Interpreter::visitLiteral(const Expr::LiteralExpr *expr) {
  _value = std::visit(Token::variant_value_getter(), expr->value());
}

void Interpreter::visitGrouping(const Expr::GroupingExpr *expr) {
  evalutate(expr->expr());
}

void Interpreter::visitUnary(const Expr::UnaryExpr *expr) {
  evalutate(expr->right());

  switch (expr->op().type()) {
  case MINUS:
    _value = -std::any_cast<double>(_value);
    break;
  case BANG:
    _value = !isTruthyVal(_value);
    break;
  default:
    throw RuntimeError(expr->op(), "Invalid operator to Unary expression");
    break;
  }
}

void Interpreter::visitBinary(const Expr::BinaryExpr *expr) {
  evalutate(expr->left());
  std::any left = _value;
  evalutate(expr->right());
  std::any right = _value;

  switch (expr->op().type()) {
  case GREATER:
    enforceDouble(expr->op(), right);
    _value = std::any_cast<double>(left) > std::any_cast<double>(right);
    break;
  case GREATER_EQUAL:
    enforceDouble(expr->op(), right);
    _value = std::any_cast<double>(left) >= std::any_cast<double>(right);
    break;
  case LESS:
    enforceDouble(expr->op(), right);
    _value = std::any_cast<double>(left) < std::any_cast<double>(right);
    break;
  case LESS_EQUAL:
    enforceDouble(expr->op(), right);
    _value = std::any_cast<double>(left) <= std::any_cast<double>(right);
    break;
  case MINUS:
    enforceDouble(expr->op(), right);
    _value = std::any_cast<double>(left) - std::any_cast<double>(right);
    break;
  case PLUS:
    if (isOfType<std::string>(left) && isOfType<std::string>(right)) {
      _value =
          std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
    } else if (isOfType<double>(left) && isOfType<double>(right)) {
      _value = std::any_cast<double>(left) + std::any_cast<double>(right);
    } else {
      throw RuntimeError(expr->op(),
                         "Operands must both be numbers or strings");
    }
    break;
  case SLASH:
    if (std::any_cast<double>(right) == 0)
      throw RuntimeError(expr->op(), "Division by Zero");
    _value = std::any_cast<double>(left) / std::any_cast<double>(right);
    break;
  case STAR:
    _value = std::any_cast<double>(left) * std::any_cast<double>(right);
    break;
  case BANG_EQUAL:
    _value = !isEqual(left, right);
    break;
  case EQUAL_EQUAL:
    _value = isEqual(left, right);
    break;
  default:
    throw RuntimeError(expr->op(), "Invalid operator for binary expression");
  }
}

void Interpreter::visitTernary(const Expr::TernaryExpr *expr) {
  if (isTruthyExpr(expr->condition())) {
    evalutate(expr->first());
  } else {
    evalutate(expr->second());
  }
}

void Interpreter::visitVariable(const Expr::VariableExpr *expr) {
  _value = _environment.get(expr->name());
}

void Interpreter::visitAssign(const Expr::AssignExpr *expr) {
  evalutate(expr->value());
  _environment.assign(expr->name(), _value);
}

void Interpreter::visitLogic(const Expr::LogicExpr *expr) {
  switch (expr->op().type()) {
  case OR:
    if (!isTruthyExpr(expr->first()))
      _value = isTruthyExpr(expr->second());
    else
      _value = true;
    break;
  case AND:
    if (isTruthyExpr(expr->first()))
      _value = isTruthyExpr(expr->second());
    else
      _value = false;
    break;
  default:
    throw RuntimeError(expr->op(), "Invalid operator for logic expression");
  }
}

void Interpreter::visitCall(const Expr::CallExpr *expr) {
  std::any callee = eval(expr->callee());

  std::vector<std::any> args;
  for (const Expr::Expr *arg : expr->arguments()) {
    args.push_back(eval(arg));
  }

  if (callee.type() != typeid(LoxFunction))
    throw RuntimeError(expr->paren(), "Can only call function or classes.");

  LoxFunction function = std::any_cast<LoxFunction>(callee);

  if (args.size() != function.arity()) {
    std::stringstream error;
    error << "Expected" << function.arity() << "arguments but got "
          << args.size() << ".";
    throw RuntimeError(expr->paren(), error.str());
  }
  _value = function.call(this, args);
}

void Interpreter::evalutate(const Expr::Expr *expr) { expr->accept(this); }

void Interpreter::execute(const Stmt::Stmt *statement) {
  statement->accept(this);
}

void Interpreter::executeBlock(
    Environment env, const std::vector<const Stmt::Stmt *> &statements) {
  Environment prev = _environment;
  try {
    _environment = env;

    for (const Stmt::Stmt *statement : statements) {
      execute(statement);
    }
    _environment = prev;
  } catch (RuntimeError err) {
    _environment = prev;
    throw err;
  }
}

void Interpreter::enforceDouble(Token op, const std::any &val) {
  if (isOfType<double>(val))
    return;
  throw RuntimeError(op, "Operand must be a number.");
}

bool Interpreter::isTruthyExpr(const Expr::Expr *expr) {
  evalutate(expr);
  return isTruthyVal(_value);
}

bool Interpreter::isTruthyVal(const std::any &val) {
  if (!val.has_value())
    return false;

  if (isOfType<bool>(val))
    return std::any_cast<bool>(val);
  else if (isOfType<double>(val))
    return std::any_cast<double>(val) != 0;
  else if (isOfType<std::string>(val))
    return std::any_cast<std::string>(val).size() != 0;

  throw RuntimeError(Token{END_OF_FILE, ""},
                     "Cannot determine if value is truthy");
}

bool Interpreter::isEqual(const std::any &first, const std::any &second) {
  if (!first.has_value() && !second.has_value())
    return true;
  if (!first.has_value() || !second.has_value())
    return false;

  if (first.type() == second.type()) {
    if (isOfType<std::string>(first)) {
      return std::any_cast<std::string>(first) ==
             std::any_cast<std::string>(second);
    } else if (isOfType<double>(first)) {
      return std::any_cast<double>(first) == std::any_cast<double>(second);
    }
  }

  return false;
}

template <typename T> bool Interpreter::isOfType(const std::any &val) {
  return val.type() == typeid(T);
}
