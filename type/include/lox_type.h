#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <typeindex>
#include <variant>

#include <invalid_type_exception.h>
#include <iostream>

class LoxFunction;
class LoxInstance;
class LoxClass;
class LoxCallable;

class LoxType {
public:
  LoxType();
  LoxType(const LoxType &);
  LoxType(std::monostate);
  LoxType(bool);
  LoxType(double);
  LoxType(std::string);
  LoxType(LoxCallable *);
  LoxType(LoxFunction *);
  LoxType(LoxClass *);
  LoxType(LoxInstance *);

  template <typename T> const T &getValue() const;

  template <typename T> bool isType() const;

  bool empty() const;

  LoxType &operator=(LoxType &);
  LoxType &operator=(bool);
  LoxType &operator=(double);
  LoxType &operator=(std::string);
  LoxType &operator=(LoxCallable *);
  LoxType &operator=(LoxFunction *);
  LoxType &operator=(LoxClass *);
  LoxType &operator=(LoxInstance *);

  LoxType &operator=(const LoxType &);

  bool operator==(const LoxType &) const;

private:
  std::variant<bool, double, std::string, LoxInstance *, LoxCallable *,
               LoxClass *, LoxFunction *, std::monostate>
      _value;
  std::type_index _type;

  friend std::ostream &operator<<(std::ostream &, const LoxType &);
};

template <typename T> const T &LoxType::getValue() const {
  if (isType<T>()) {
    return std::get<T>(_value);
  }

  throw InvalidTypeException(_type, typeid(T));
}

template <typename T> bool LoxType::isType() const {
  std::type_index template_type = typeid(T);

  return template_type == _type;
}
