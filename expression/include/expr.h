#pragma once

#include <token.h>
#include <any>
#include <memory>
#include <utility>

namespace Expr {

class Expr;
class ExprVisitor;

typedef std::shared_ptr<Expr> expr_ptr;

class Expr {
public:

  virtual void accept(ExprVisitor*) const = 0;
};


class BinaryExpr : public Expr, public std::enable_shared_from_this<BinaryExpr> {
public:

  BinaryExpr (expr_ptr left, Token op, expr_ptr right)
  : _left(std::move(left)), _op(std::move(op)), _right(std::move(right)) {}
  
  void accept(ExprVisitor* visitor) const override;
  
  expr_ptr left()  const {return _left;}
  Token op()    const {return _op;}
  expr_ptr right() const {return _right;}
private:
  const expr_ptr _left;
  const Token _op;
  const expr_ptr _right;
};


class LiteralExpr : public Expr, public std::enable_shared_from_this<LiteralExpr> {
public:
  explicit LiteralExpr (Token::literal_variant value)
  : _value(std::move(value)) {}

  void accept(ExprVisitor* visitor) const override;

  Token::literal_variant value() const {return _value;}
private:
  const Token::literal_variant _value;
};


class UnaryExpr : public Expr, public std::enable_shared_from_this<UnaryExpr> {
public:
  UnaryExpr (Token op, expr_ptr right)
  : _op(std::move(op)), _right(std::move(right)) {}
 
  void accept(ExprVisitor* visitor) const override;

  Token op() const {return _op;}
  expr_ptr right() const {return _right;}
private:
  const Token _op;
  const expr_ptr _right;
};


class GroupingExpr : public Expr, public std::enable_shared_from_this<GroupingExpr> {
public:
  explicit GroupingExpr (expr_ptr expression)
  : _expression(std::move(expression)) {}

  void accept(ExprVisitor* visitor) const override;

  expr_ptr expr() const {return _expression;}
private:
  const expr_ptr _expression;
};

}
