#include <cstdlib>
#include <sstream>

#include <lox_type.h>
#include <lox_instance.h>
#include <lox_class.h>
#include <lox_callable.h>
#include <lox_function.h>

LoxType::LoxType() : _value(std::monostate()), _type(typeid(std::monostate)) {}
LoxType::LoxType(const LoxType& other) : _value(other._value), _type(other._type) {}
LoxType::LoxType(std::monostate val) : LoxType() {}

LoxType::LoxType(bool val) : _value(val), _type(typeid(bool)) {}
LoxType::LoxType(double val) : _value(val), _type(typeid(double)) {}
LoxType::LoxType(std::string val) : _value(val), _type(typeid(std::string)) {}
LoxType::LoxType(LoxCallable* val)
    : _value(val), _type(typeid(LoxCallable*)) {}
LoxType::LoxType(LoxFunction* val)
    : _value(val), _type(typeid(LoxFunction*)) {}
LoxType::LoxType(LoxClass* val)
    : _value(val), _type(typeid(LoxClass*)) {}
LoxType::LoxType(LoxInstance* val)
    : _value(val), _type(typeid(LoxInstance*)) {}

bool LoxType::empty() const {
  return isType<std::monostate>();
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

LoxType &LoxType::operator=(LoxCallable* other) {
  _value = std::move(other);
  _type = typeid(LoxCallable*);

  return *this;
}

LoxType &LoxType::operator=(LoxInstance* other) {
  _value = std::move(other);
  _type = typeid(LoxInstance*);

  return *this;
}

LoxType &LoxType::operator=(LoxClass* other) {
  _value = std::move(other);
  _type = typeid(LoxClass*);

  return *this;
}

LoxType &LoxType::operator=(LoxFunction* other) {
  _value = std::move(other);
  _type = typeid(LoxFunction*);

  return *this;
}

LoxType &LoxType::operator=(const LoxType& other) {
  _value = other._value;
  _type = other._type;

  return *this;
}

bool LoxType::operator==(const LoxType& other) const {
  return (other._type == _type) && (other._value == _value);
}

struct Printer {
  std::string operator()(bool val) {return val ? "true" : "false";}
  std::string operator()(double val) {return std::to_string(val);}
  std::string operator()(std::string val) {return val;}
  std::string operator()(std::monostate val) {return "nil";}
  std::string operator()(const LoxCallable* val) {return "<Lox Function>";}
  std::string operator()(const LoxInstance* val) {return "<Lox Instance>";}
};

std::ostream& operator<<(std::ostream& outs, const LoxType& type) {
  outs << std::visit(Printer(), type._value);
  return outs;
}

