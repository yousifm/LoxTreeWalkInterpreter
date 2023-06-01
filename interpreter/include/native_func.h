#include <ctime>
#include <lox_callable.h>

class Clock : public LoxCallable {
public:
  std::any call(Interpreter* interpreter, const std::vector<std::any>& args) const override {
    return time(nullptr) / 1000.0;
  }
  size_t arity() const override {return 0;}
};
