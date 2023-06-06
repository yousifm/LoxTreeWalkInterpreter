#include <variant>
#include <string>
#include <typeindex>
#include <memory>

#include <lox_callable.h>

class LoxType {
private:
  typedef std::unique_ptr<LoxCallable> callable_ptr ;
public:
  LoxType();
  LoxType(bool);
  LoxType(double);
  LoxType(std::string);
  LoxType(callable_ptr); 
  
  template <typename T>
  T getValue();

  LoxType& operator=(LoxType&);
  LoxType& operator=(bool);
  LoxType& operator=(double);
  LoxType& operator=(std::string);
  LoxType& operator=(callable_ptr);
private:
  std::variant<bool, double, std::string, callable_ptr, std::monostate> _value;
  std::type_index _type;
};
