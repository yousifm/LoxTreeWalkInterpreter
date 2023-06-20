#pragma once

#include <lox_callable.h>

#include <string>

class LoxClass : public LoxCallable {
public:
  LoxClass(const std::string& name): _name(name) {}

  LoxType call(Interpreter *, const std::vector<LoxType> &) override;
  size_t arity() const override { return 0; }

  const std::string& name();
private:
  std::string _name;
};
