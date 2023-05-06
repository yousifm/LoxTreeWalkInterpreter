#pragma once

#include "expr.h"

namespace Expr{

class Unary : public Expr {
public:
  Unary (Token op, Expr& right)
  : _op(op), _right(right) {}

private:
  const Token _op;
  const Expr& _right;
};

}
