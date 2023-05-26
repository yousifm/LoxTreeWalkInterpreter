#pragma once

#include <any>
#include <memory>
#include <token.h>
#include <utility>

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
  BinaryExpr(const Expr* left, Token op, const Expr* right)
      : _left(left), _op(std::move(op)), _right(right) {}

  void accept(ExprVisitor *visitor) const override;

  const Expr* left() const { return _left.get(); }
  Token op() const { return _op; }
  const Expr* right() const { return _right.get(); }

private:
  const expr_ptr _left;
  const Token _op;
  const expr_ptr _right;
};

class LiteralExpr : public Expr {
public:
  explicit LiteralExpr(Token::literal_variant value)
      : _value(std::move(value)) {}

  void accept(ExprVisitor *visitor) const override;

  Token::literal_variant value() const { return _value; }

private:
  const Token::literal_variant _value;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token op, const Expr* right)
      : _op(std::move(op)), _right(right) {}

  void accept(ExprVisitor *visitor) const override;

  Token op() const { return _op; }
  const Expr* right() const { return _right.get(); }

private:
  const Token _op;
  const expr_ptr _right;
};

class GroupingExpr : public Expr {
public:
  explicit GroupingExpr(const Expr* expression)
      : _expression(expression) {}

  void accept(ExprVisitor *visitor) const override;

  const Expr* expr() const { return _expression.get(); }

private:
  const expr_ptr _expression;
};

class TernaryExpr : public Expr {
public:
  explicit TernaryExpr(const Expr* condition, const Expr* first, const Expr* second)
      : _condition(condition), _first(first),
        _second(second) {}

  void accept(ExprVisitor *visitor) const override;

  const Expr* condition() const { return _condition.get(); }
  const Expr* first() const { return _first.get(); }
  const Expr* second() const { return _second.get(); }

private:
  const expr_ptr _condition;
  const expr_ptr _first;
  const expr_ptr _second;
};

class VariableExpr : public Expr {
public:
  explicit VariableExpr(Token name) : _name(std::move(name)) {}
  void accept(ExprVisitor *visitor) const override;
  const Token& name() const {return _name;}
private:
  Token _name;
};

} // namespace Expr
