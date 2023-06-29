#include <lox_class.h>
#include <lox_instance.h>
#include <lox_type.h>

LoxType LoxClass::call(Interpreter *interpreter,
                       const std::vector<LoxType> &args) {
  LoxInstance *instance = new LoxInstance(this);

  std::optional<LoxFunction> initializer = getMethod("init");
  if (initializer.has_value()) {
    initializer.value().bind(instance)->call(interpreter, args);
  }

  return LoxType(instance);
}

const std::string &LoxClass::name() { return _name; }

std::optional<LoxFunction> LoxClass::getMethod(const std::string &name) const {
  if (_methods.contains(name))
    return std::optional(_methods.at(name));

  return std::nullopt;
}

size_t LoxClass::arity() const {
  std::optional<LoxFunction> initializer = getMethod("init");

  if (initializer.has_value())
    return initializer->arity();

  return 0;
}
