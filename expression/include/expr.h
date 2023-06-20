#pragma once

#include <token.h>

#include <any>
#include <memory>
#include <utility>
#include <vector>

namespace Expr {

class Expr;
class ExprVisitor;

typedef std::unique_ptr<const Expr> expr_ptr;

class Expr {
public:
  virtual void accept(ExprVisitor *) const = 0;
};

class BinaryExpr : public Expr,
                   public std::enable_shared_from_this<BinaryExpr> {
public:
  BinaryExpr(const Expr *left, Token op, const Expr *right)
      : _left(left), _op(std::move(op)), _right(right) {}

  void accept(ExprVisitor *) const override;

  const Expr *left() const { return _left.get(); }
  Token op() const { return _op; }
  const Expr *right() const { return _right.get(); }

private:
  const expr_ptr _left;
  const Token _op;
  const expr_ptr _right;
};

class LiteralExpr : public Expr {
public:
  explicit LiteralExpr(LoxType value) : _value(std::move(value)) {}

  void accept(ExprVisitor *) const override;

  LoxType value() const { return _value; }

private:
  const LoxType _value;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token op, const Expr *right) : _op(std::move(op)), _right(right) {}

  void accept(ExprVisitor *) const override;

  Token op() const { return _op; }
  const Expr *right() const { return _right.get(); }

private:
  const Token _op;
  const expr_ptr _right;
};

class GroupingExpr : public Expr {
public:
  explicit GroupingExpr(const Expr *expression) : _expression(expression) {}

  void accept(ExprVisitor *) const override;

  const Expr *expr() const { return _expression.get(); }

private:
  const expr_ptr _expression;
};

class TernaryExpr : public Expr {
public:
  explicit TernaryExpr(const Expr *condition, const Expr *first,
                       const Expr *second)
      : _condition(condition), _first(first), _second(second) {}

  void accept(ExprVisitor *) const override;

  const Expr *condition() const { return _condition.get(); }
  const Expr *first() const { return _first.get(); }
  const Expr *second() const { return _second.get(); }

private:
  const expr_ptr _condition;
  const expr_ptr _first;
  const expr_ptr _second;
};

class VariableExpr : public Expr {
public:
  explicit VariableExpr(Token name) : _name(std::move(name)) {}
  void accept(ExprVisitor *) const override;
  const Token &name() const { return _name; }

private:
  Token _name;
};

class AssignExpr : public Expr {
public:
  explicit AssignExpr(Token name, Expr *value)
      : _name(std::move(name)), _value(std::move(value)) {}

  void accept(ExprVisitor *) const override;

  const Token name() const { return _name; }
  const Expr *value() const { return _value; }

private:
  Token _name;
  Expr *_value;
};

class LogicExpr : public Expr {
public:
  explicit LogicExpr(Token op, Expr *first, Expr *second)
      : _op(op), _first(first), _second(second) {}

  void accept(ExprVisitor *visitor) const override;

  const Token op() const { return _op; }
  const Expr *first() const { return _first; }
  const Expr *second() const { return _second; }

private:
  Token _op;
  Expr *_first;
  Expr *_second;
};

class CallExpr : public Expr {
public:
  explicit CallExpr(Expr *callee, Token paren,
                    const std::vector<Expr *> &arguments)
      : _callee(callee), _paren(paren), _arguments(arguments){};
  void accept(ExprVisitor *) const override;

  const Expr *callee() const { return _callee; }
  const Token paren() const { return _paren; }
  const std::vector<Expr *> arguments() const { return _arguments; }

private:
  Expr *_callee;
  Token _paren;
  const std::vector<Expr *> _arguments;
};

class GetExpr : public Expr {
public:
  explicit GetExpr(Expr *object, Token name) : _object(object), _name(name) {}
  void accept(ExprVisitor *) const override;

  const Expr *object() const { return _object; }
  const Token name() const { return _name; }

private:
  Expr *_object;
  Token _name;
};

} // namespace Expr
