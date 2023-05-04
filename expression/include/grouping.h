#include "expr.h"

namespace Expr{

class Grouping : public Expr {
public:
  Grouping (Expr& expression)
  : _expression(expression) {}

private:
  const Expr& _expression;
};

}