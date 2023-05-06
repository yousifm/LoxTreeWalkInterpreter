#pragma once

#include <token.h>
#include <any>

namespace Expr {

class Expr {
public:
  explicit Expr();
  
  virtual void accept(ExprVisitor&) const = 0;
};

}
