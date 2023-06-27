#include "interpreter.h"
#include "lox.h"
#include "lox_callable.h"
#include "lox_class.h"
#include "native_func.h"
#include "return.h"
#include "runtime_error.h"
#include "token_type.h"
#include <lox_instance.h>

#include <sstream>

Interpreter::Interpreter() {
  _globals = std::make_shared<Environment>();

  _globals->define("clock", LoxType(new Clock()));
  _environment = _globals;
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

LoxType Interpreter::eval(const Expr::Expr *expr) {
  evalutate(expr);
  return _value;
}

void Interpreter::visitExprStmt(const Stmt::ExprStmt *stmt) {
  evalutate(stmt->expr());
}

void Interpreter::visitPrintStmt(const Stmt::PrintStmt *stmt) {
  LoxType val = eval(stmt->expr());
  std::cout << val << std::endl;
}

void Interpreter::visitVarStmt(const Stmt::VarStmt *stmt) {
  LoxType val;

  if (stmt->init() != nullptr) {
    val = eval(stmt->init());
  }

  _environment->define(stmt->name().lexeme(), val);
}

void Interpreter::visitBlock(const Stmt::Block *block) {
  std::shared_ptr<Environment> env =
      std::make_shared<Environment>(_environment);

  executeBlock(env, block->statements());
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
  LoxType function = new LoxFunction(*stmt, _environment);
  _environment->define(stmt->name().lexeme(), function);
}

void Interpreter::visitReturnStmt(const Stmt::ReturnStmt *stmt) {
  LoxType val;

  if (stmt->expr() != nullptr)
    val = eval(stmt->expr());

  throw Return(_value);
}

void Interpreter::visitClassStmt(const Stmt::ClassStmt *stmt) {
  _environment->define(stmt->name().lexeme(), 0.0);

  std::map<std::string, LoxFunction> methods;
  for (Stmt::FunctionStmt *method : stmt->methods()) {
    methods.insert(
        {method->name().lexeme(), LoxFunction(*method, _environment)});
  }

  LoxType loxClass(new LoxClass(stmt->name().lexeme(), methods));

  _environment->assign(stmt->name(), loxClass);
}

void Interpreter::visitLiteral(const Expr::LiteralExpr *expr) {
  _value = expr->value();
}

void Interpreter::visitGrouping(const Expr::GroupingExpr *expr) {
  evalutate(expr->expr());
}

void Interpreter::visitUnary(const Expr::UnaryExpr *expr) {
  evalutate(expr->right());

  switch (expr->op().type()) {
  case MINUS:
    _value = -_value.getValue<double>();
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
  LoxType left = _value;
  evalutate(expr->right());
  LoxType right = _value;

  switch (expr->op().type()) {
  case GREATER:
    enforceDouble(expr->op(), right);
    _value = left.getValue<double>() > right.getValue<double>();
    break;
  case GREATER_EQUAL:
    enforceDouble(expr->op(), right);
    _value = left.getValue<double>() >= right.getValue<double>();
    break;
  case LESS:
    enforceDouble(expr->op(), right);
    _value = left.getValue<double>() < right.getValue<double>();
    break;
  case LESS_EQUAL:
    enforceDouble(expr->op(), right);
    _value = left.getValue<double>() <= right.getValue<double>();
    break;
  case MINUS:
    enforceDouble(expr->op(), right);
    _value = left.getValue<double>() - right.getValue<double>();
    break;
  case PLUS:
    if (left.isType<std::string>() && right.isType<std::string>()) {
      _value = left.getValue<std::string>() + right.getValue<std::string>();
    } else if (left.isType<double>() && right.isType<double>()) {
      _value = left.getValue<double>() + right.getValue<double>();
    } else {
      throw RuntimeError(expr->op(),
                         "Operands must both be numbers or strings");
    }
    break;
  case SLASH:
    if (right.getValue<double>() == 0)
      throw RuntimeError(expr->op(), "Division by Zero");
    _value = left.getValue<double>() / right.getValue<double>();
    break;
  case STAR:
    _value = left.getValue<double>() * right.getValue<double>();
    break;
  case BANG_EQUAL:
    _value = left != right;
    break;
  case EQUAL_EQUAL:
    _value = left == right;
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
  _value = lookupVariable(expr->name(), expr);
}

void Interpreter::visitAssign(const Expr::AssignExpr *expr) {
  evalutate(expr->value());

  if (_locals.contains(expr)) {
    int distance = _locals[expr];
    _environment->assignAt(distance, expr->name(), _value);
  } else {
    _globals->assign(expr->name(), _value);
  }
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
  LoxType callee = eval(expr->callee());

  std::vector<LoxType> args;
  for (const Expr::Expr *arg : expr->arguments()) {
    args.push_back(eval(arg));
  }

  LoxCallable *function;

  if (callee.isType<LoxFunction *>())
    function = callee.getValue<LoxFunction *>();
  else if (callee.isType<LoxCallable *>())
    function = callee.getValue<LoxCallable *>();
  else if (callee.isType<LoxClass *>())
    function = callee.getValue<LoxClass *>();
  else
    throw RuntimeError(expr->paren(), "Can only call functions or classes.");

  if (args.size() != function->arity()) {
    std::stringstream error;
    error << "Expected " << function->arity() << " arguments but got "
          << args.size() << ".";
    throw RuntimeError(expr->paren(), error.str());
  }
  _value = function->call(this, args);
}

void Interpreter::visitGet(const Expr::GetExpr *expr) {
  LoxType object = eval(expr->object());
  if (object.isType<LoxInstance *>()) {
    _value = object.getValue<LoxInstance *>()->get(expr->name());
    return;
  }

  throw RuntimeError(expr->name(), "Cannot get property of non-instance.");
}

void Interpreter::visitSet(const Expr::SetExpr *expr) {
  LoxType object = eval(expr->object());

  if (object.isType<LoxInstance *>()) {
    LoxType val = eval(expr->value());

    object.getValue<LoxInstance *>()->set(expr->name(), val);

    return;
  }

  throw RuntimeError(expr->name(), "Cannot set property of non-instance.");
}

void Interpreter::resolve(const Expr::Expr *expr, int depth) {
  _locals[expr] = depth;
}

void Interpreter::evalutate(const Expr::Expr *expr) { expr->accept(this); }

void Interpreter::execute(const Stmt::Stmt *statement) {
  statement->accept(this);
}

void Interpreter::executeBlock(
    std::shared_ptr<Environment> env,
    const std::vector<const Stmt::Stmt *> &statements) {
  std::shared_ptr<Environment> prev = _environment;

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

void Interpreter::enforceDouble(Token op, const LoxType &val) {
  if (val.isType<double>())
    return;
  throw RuntimeError(op, "Operand must be a number.");
}

bool Interpreter::isTruthyExpr(const Expr::Expr *expr) {
  evalutate(expr);
  return isTruthyVal(_value);
}

bool Interpreter::isTruthyVal(const LoxType &val) {
  if (val.empty())
    return false;

  if (val.isType<bool>())
    return val.getValue<bool>();
  else if (val.isType<double>())
    return val.getValue<double>() != 0;
  else if (val.isType<std::string>())
    return val.getValue<std::string>().size() != 0;

  throw RuntimeError(Token{END_OF_FILE, ""},
                     "Cannot determine if value is truthy");
}

LoxType Interpreter::lookupVariable(const Token &name, const Expr::Expr *expr) {
  if (_locals.contains(expr)) {
    int distance = _locals[expr];

    return _environment->getAt(distance, name);
  }
  return _globals->get(name);
}
