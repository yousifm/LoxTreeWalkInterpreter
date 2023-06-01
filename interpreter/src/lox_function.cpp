#include <lox_function.h>
#include <interpreter.h>

LoxFunction::LoxFunction(const Stmt::FunctionStmt declaration)
    : _declaration(declaration) {}

std::any LoxFunction::call(Interpreter *interpreter,
                           const std::vector<std::any> &args) const {
  Environment environment{&interpreter->globals};

  for (size_t i = 0; i < _declaration.params().size(); i++) {
    environment.define(_declaration.params().at(i).lexeme(), args.at(i));
  }
  
  interpreter->executeBlock(environment, _declaration.body());
  return nullptr;
}

size_t LoxFunction::arity() const {
  return _declaration.params().size();
}
