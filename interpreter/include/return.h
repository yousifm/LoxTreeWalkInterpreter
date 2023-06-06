#include <any>
#include <exception>
#include <lox_type.h>

class Return : std::exception {
public:
  Return(LoxType val) : _value(val) {}
  LoxType value() { return _value; }

private:
  LoxType _value;
};
