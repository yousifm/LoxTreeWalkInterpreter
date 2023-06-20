#pragma once

#include <ostream>
#include <variant>
#include <string>
#include <typeindex>
#include <memory>

#include <lox_callable.h>
#include <invalid_type_exception.h>
#include <lox_instance.h>

class LoxType {
public:
  typedef std::shared_ptr<LoxCallable> callable_ptr ;

  LoxType();
  LoxType(const LoxType&);
  LoxType(std::monostate);
  LoxType(bool);
  LoxType(double);
  LoxType(std::string);
  LoxType(callable_ptr); 
  LoxType(LoxInstance);
  
  template <typename T>
  const T& getValue() const;

  template <typename T>
  bool isType() const;

  bool empty() const;

  LoxType& operator=(LoxType&);
  LoxType& operator=(bool);
  LoxType& operator=(double);
  LoxType& operator=(std::string);
  LoxType& operator=(callable_ptr);

  LoxType& operator=(const LoxType&);

  bool operator==(const LoxType&) const;
private:
  std::variant<bool, double, std::string, callable_ptr, LoxInstance, std::monostate> _value;
  std::type_index _type;

  friend std::ostream& operator<<(std::ostream&, const LoxType&);
};

template <typename T>
const T& LoxType::getValue() const {
  if (isType<T>()) {
    return std::get<T>(_value);
  }

  throw InvalidTypeException(_type, typeid(T));
}

template <typename T>
bool LoxType::isType() const {
  std::type_index template_type = typeid(T);

  return template_type == _type;
}
