#pragma once

#include <runtime_error.h>
#include <token.h>
#include <unordered_map>

class Environment {
public:
  explicit Environment() : _enclosing(nullptr) {}
  explicit Environment(std::unordered_map<std::string, std::any> vals) : _values(vals) {}
  Environment(const Environment& other) : _values(other._values), _enclosing(other._enclosing) {}
  explicit Environment(Environment *enclosing) : _enclosing(enclosing) {}

  void define(std::string name, std::any value) { _values[name] = value; }

  void assign(const Token name, std::any value) {
    if (_values.contains(name.lexeme())) {
      _values[name.lexeme()] = value;
    } else if (_enclosing != nullptr) {
      _enclosing->assign(name, value);
    } else {
      throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
    }
  }

  std::any get(Token name) const {
    if (_values.contains(name.lexeme())) {
      std::any val = _values.at(name.lexeme());
      if (val.type() == typeid(std::monostate()) || val.type() == typeid(nullptr)) {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
      }
      return _values.at(name.lexeme());
    } else if (_enclosing != nullptr) {
      return _enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
  }
  
  Environment enclosing() {return *_enclosing;}
  Environment& operator=(const Environment other) {
    _values = other._values;
    _enclosing = other._enclosing;
    return *this;
  }
private:
  std::unordered_map<std::string, std::any> _values;
  Environment *_enclosing;
};
