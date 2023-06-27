#pragma once

#include <lox_callable.h>
#include <lox_function.h>

#include <map>
#include <string>
#include <optional>

class LoxClass : public LoxCallable {
public:
  LoxClass(const std::string &name,
           const std::map<std::string, LoxFunction> &methods)
      : _name(name), _methods(std::move(methods)) {}

  LoxType call(Interpreter *, const std::vector<LoxType> &) override;
  size_t arity() const override { return 0; }

  const std::string &name();

  std::optional<LoxFunction> getMethod(const std::string &);

private:
  std::string _name;
  std::map<std::string, LoxFunction> _methods;
};
