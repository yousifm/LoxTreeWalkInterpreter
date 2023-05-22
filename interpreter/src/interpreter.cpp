#include "interpreter.h"
#include "runtime_error.h"
#include "token_type.h"
#include <any>

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
    _value = std::any_cast<double>(_value);
    break;
  case BANG:
    _value = !isTruthy(_value);
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
      throw RuntimeError(expr->op(), "Operands must both be numbers or strings");
    }
    break;
  case SLASH:
    if (std::any_cast<double>(right) == 0) throw RuntimeError(expr->op(), "Division by Zero");
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
  if (isTruthy(expr->condition())) {
    evalutate(expr->first());
  } else {
    evalutate(expr->second());
  }
}

std::any Interpreter::eval(const Expr::Expr *expr) {
  evalutate(expr);
  return _value;
}

void Interpreter::evalutate(const Expr::Expr *expr) { expr->accept(this); }

void Interpreter::enforceDouble(Token op, const std::any &val) {
  if (isOfType<double>(val))
    return;
  throw RuntimeError(op, "Operand must be a number.");
}

bool Interpreter::isTruthy(const Expr::Expr* expr) {
  evalutate(expr);
  return isTruthy(_value);
}

bool Interpreter::isTruthy(const std::any &val) {
  if (!val.has_value())
    return false;

  if (isOfType<bool>(val))
    return std::any_cast<bool>(val);
  return true;
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
