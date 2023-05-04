#include "expr.h"

namespace Expr{

class Binary : public Expr {
public:
  Binary (Expr& left, Token op, Expr& right)
  : _left(left), _op(op), _right(right) {}

private:
  const Expr& _left;
  const Token _op;
  const Expr& _right;
};

}