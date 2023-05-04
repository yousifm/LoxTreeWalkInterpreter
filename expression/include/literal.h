#include "expr.h"

namespace Expr{

class Literal : public Expr {
public:
  Literal (Token::literal_variant value)
  : _value(value) {}

private:
  const Token::literal_variant _value;
};

}
