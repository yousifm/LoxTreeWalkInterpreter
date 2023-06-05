#include <any>
#include <exception>

class Return : std::exception {
public:
  Return(std::any val) : _value(val) {}
  std::any value() { return _value; }

private:
  std::any _value;
};
