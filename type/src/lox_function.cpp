#include <lox_function.h>
#include <interpreter.h>
#include <return.h>

LoxFunction::LoxFunction(const Stmt::FunctionStmt declaration, Environment closure)
    : _declaration(declaration), _closure(closure) {}

LoxType LoxFunction::call(Interpreter *interpreter,
                           const std::vector<LoxType> &args) const {
  Environment environment{_closure};

  for (size_t i = 0; i < _declaration.params().size(); i++) {
    environment.define(_declaration.params().at(i).lexeme(), args.at(i));
  }
  
  try {
    interpreter->executeBlock(environment, _declaration.body());
  } catch (Return r) {
    return r.value();
  }

  return LoxType(std::monostate());
}

size_t LoxFunction::arity() const {
  return _declaration.params().size();
}
