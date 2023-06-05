#pragma once

#include <variant>
#include <string>
#include <typeindex>

#include <lox_callable.h>

class LoxType {
public:
  struct Getter {
    std::monostate operator()(const std::monostate val) const {return val;}
    std::string operator()(const std::string& val) const {return val;}
    double operator()(double val) const { return val;}
    bool operator()(bool val) const { return val;}
    LoxCallable* operator()(LoxCallable* val) {return val;}
  };

  LoxType() : _value(std::monostate()), _type(typeid(std::monostate())) {}
  LoxType(std::monostate none) : _value(none), _type(typeid(std::monostate)) {}
  LoxType(std::string str) : _value(str), _type(typeid(std::string)) {}
  LoxType(double number)   : _value(number), _type(typeid(double)) {}
  LoxType(bool boolean)    : _value(boolean), _type(typeid(bool)) {}
  LoxType(LoxCallable* callable) : _value(callable), _type(typeid(LoxCallable*)) {}

  double getDouble() {return std::visit(Getter(), _value);};

  LoxType& operator=(const LoxType& other) {
    _value = other._value;
    _type = other._type;

    return *this;
  }
  
  LoxType& operator=(std::string str) {
    _value = str;
    _type = typeid(std::string);

    return *this;
  }

  LoxType& operator=(double number) {
    _value = number;
    _type = typeid(double);

    return *this;
  }

  LoxType& operator=(bool boolean) {
    _value = boolean;
    _type = typeid(bool);

    return *this;
  }

  LoxType& operator=(LoxCallable* callable) {
    _value = callable;
    _type = typeid(LoxCallable*);
    
    return *this;
  }

private:
  std::variant<std::monostate, std::string, double, bool, LoxCallable*> _value;
  std::type_index _type;
};
