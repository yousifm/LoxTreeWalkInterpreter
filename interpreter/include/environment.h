#pragma once

#include <runtime_error.h>
#include <token.h>
#include <unordered_map>

class Environment {
public:
  void define(std::string name, std::any value) { _values[name] = value; }

  void assign(const Token name, std::any value) {
    if (_values.contains(name.lexeme())) {
      _values[name.lexeme()] = value;
    } else {
      throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
    }
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
