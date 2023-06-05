#include <lox_function.h>
#include <interpreter.h>
#include <return.h>

LoxFunction::LoxFunction(const Stmt::FunctionStmt declaration)
    : _declaration(declaration) {}

std::any LoxFunction::call(Interpreter *interpreter,
                           const std::vector<std::any> &args) const {
  Environment environment{&interpreter->globals};

  for (size_t i = 0; i < _declaration.params().size(); i++) {
    environment.define(_declaration.params().at(i).lexeme(), args.at(i));
  }
  
  try {
    interpreter->executeBlock(environment, _declaration.body());
  } catch (Return r) {
    return r.value();
  }

  return nullptr;
}

size_t LoxFunction::arity() const {
  return _declaration.params().size();
}
