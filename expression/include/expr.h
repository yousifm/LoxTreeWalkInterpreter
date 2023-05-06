#pragma once

#include <token.h>
#include <any>
#include <memory>
#include <utility>

namespace Expr {

class ExprVisitor;

class Expr {
public:
  virtual void accept(ExprVisitor*) const = 0;
};


class BinaryExpr : public Expr, public std::enable_shared_from_this<BinaryExpr> {
public:
  BinaryExpr (std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
  : _left(std::move(left)), _op(std::move(op)), _right(std::move(right)) {}
  
  void accept(ExprVisitor* visitor) const override;
  
  [[nodiscard]] std::shared_ptr<Expr> left()  const {return _left;}
  [[nodiscard]] Token op()    const {return _op;}
  [[nodiscard]] std::shared_ptr<Expr> right() const {return _right;}
private:
  const std::shared_ptr<Expr> _left;
  const Token _op;
  const std::shared_ptr<Expr> _right;
};


class LiteralExpr : public Expr, public std::enable_shared_from_this<LiteralExpr> {
public:
  explicit LiteralExpr (Token::literal_variant value)
  : _value(std::move(value)) {}

  void accept(ExprVisitor* visitor) const override;

  [[nodiscard]] Token::literal_variant value() const {return _value;}
private:
  const Token::literal_variant _value;
};


class UnaryExpr : public Expr, public std::enable_shared_from_this<UnaryExpr> {
public:
  UnaryExpr (Token op, std::shared_ptr<Expr> right)
  : _op(std::move(op)), _right(std::move(right)) {}
 
  void accept(ExprVisitor* visitor) const override;

  [[nodiscard]] Token op() const {return _op;}
  std::shared_ptr<Expr> right() const {return _right;}
private:
  const Token _op;
  const std::shared_ptr<Expr> _right;
};


class GroupingExpr : public Expr, public std::enable_shared_from_this<GroupingExpr> {
public:
  explicit GroupingExpr (std::shared_ptr<Expr> expression)
  : _expression(std::move(expression)) {}

  void accept(ExprVisitor* visitor) const override;

  [[nodiscard]] std::shared_ptr<Expr> expr() const {return _expression;}
private:
  const std::shared_ptr<Expr> _expression;
};

}
