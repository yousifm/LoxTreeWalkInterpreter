#pragma once

#include <unordered_map>
#include <token.h>
#include <runtime_error.h>

class Environment {
public:
  void define(std::string name, std::any value) {
    _values[name] = value;
  }

  std::any get(Token name) {
    if (_values.contains(name.lexeme())) {
      return _values[name.lexeme()];
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
  }
private:
  std::unordered_map<std::string, std::any> _values;
};
