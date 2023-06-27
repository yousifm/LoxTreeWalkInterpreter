#include <lox_class.h>
#include <lox_instance.h>
#include <lox_type.h>

LoxType LoxClass::call(Interpreter *interpreter,
                       const std::vector<LoxType> &args) {
  return LoxType(new LoxInstance{this});
}

const std::string &LoxClass::name() { return _name; }

std::optional<LoxFunction> LoxClass::getMethod(const std::string &name) {
  if (_methods.contains(name)) return std::optional(_methods.at(name));

  return std::nullopt;
}

