#include <ctime>
#include <lox_callable.h>
#include <lox_type.h>

class Clock : public LoxCallable {
public:
  LoxType call(Interpreter* interpreter, const std::vector<LoxType>& args) override {
    return time(nullptr) / 1000.0;
  }
  size_t arity() const override {return 0;}
};
