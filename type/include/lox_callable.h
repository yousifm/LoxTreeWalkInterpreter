#pragma once

#include <vector>

class LoxType;
class Interpreter;

class LoxCallable {
public:
  virtual LoxType call(Interpreter *, const std::vector<LoxType> &) = 0;

  virtual size_t arity() const = 0;
};
