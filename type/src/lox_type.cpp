#include <sstream>

#include <lox_type.h>
#include <invalid_type_exception.h>

LoxType::LoxType() : _value(std::monostate()), _type(typeid(std::monostate)) {}

LoxType::LoxType(bool val) : _value(val), _type(typeid(bool)) {}
LoxType::LoxType(double val) : _value(val), _type(typeid(double)) {}
LoxType::LoxType(std::string val) : _value(val), _type(typeid(std::string)) {}
LoxType::LoxType(callable_ptr val)
    : _value(std::move(val)), _type(typeid(callable_ptr)) {}

template <typename T>
T LoxType::getValue() {
  std::type_index template_type = typeid(T);

  if (template_type != _type) {
    throw InvalidTypeException(_type, template_type);
  }

  return std::get<T>(_value);
}

LoxType &LoxType::operator=(LoxType &other) {
  _value = std::move(other._value);
  _type = other._type;

  return *this;
}

LoxType &LoxType::operator=(bool other) {
  _value = other;
  _type = typeid(bool);

  return *this;
}

LoxType &LoxType::operator=(double other) {
  _value = other;
  _type = typeid(double);

  return *this;
}

LoxType &LoxType::operator=(std::string other) {
  _value = other;
  _type = typeid(std::string);

  return *this;
}

LoxType &LoxType::operator=(callable_ptr other) {
  _value = std::move(other);
  _type = typeid(callable_ptr);

  return *this;
}
