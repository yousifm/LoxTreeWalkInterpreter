#pragma once

#include <any>
#include <vector>

class Interpreter;

class LoxCallable {
public:
  virtual std::any call(Interpreter*, const std::vector<std::any>&) const = 0;

  virtual size_t arity() const = 0;
};
