#include <lox_class.h>
#include <lox_instance.h>
#include <lox_type.h>

LoxType LoxClass::call(Interpreter* interpreter, const std::vector<LoxType>& args) {
  LoxInstance instance(this);

  return instance;
}
