#pragma once

#include <interpreter.h>

#include <any>
#include <vector>

class LoxCallable {
public:
  virtual std::any call(const Interpreter* interpreter, const std::vector<std::any>& arguments) = 0;
};
