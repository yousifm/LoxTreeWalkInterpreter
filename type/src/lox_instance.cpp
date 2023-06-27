#include <lox_instance.h>
#include <token.h>
#include <runtime_error.h>

#include <sstream>

LoxInstance::LoxInstance(LoxClass* loxClass) : _loxClass(loxClass) {}

const LoxType LoxInstance::get(Token name) const {
  if (_fields.contains(name.lexeme())) {
    return _fields.at(name.lexeme());
  }
  std::stringstream error_message;
  error_message << _loxClass->name() << " has no field named " << name.lexeme() << ".";

  throw RuntimeError(name, error_message.str());
}

void LoxInstance::set(Token name, LoxType value) {
  _fields.insert({name.lexeme(), value});
}

bool LoxInstance::operator==(const LoxInstance& other) const {
  return _fields == other._fields;
}
