#include <lox_function.h>
#include <interpreter.h>
#include <return.h>

LoxFunction::LoxFunction(const Stmt::FunctionStmt declaration, std::shared_ptr<Environment> enclosing)
    : _declaration(declaration), _closure(enclosing) {}

LoxType LoxFunction::call(Interpreter *interpreter,
                           const std::vector<LoxType> &args) {
  for (size_t i = 0; i < _declaration.params().size(); i++) {
    _closure.define(_declaration.params().at(i).lexeme(), args.at(i));
  }
  
  try {
    interpreter->executeBlock(std::make_shared<Environment>(_closure), _declaration.body());
  } catch (Return r) {
    return r.value();
  }

  return LoxType(std::monostate());
}

size_t LoxFunction::arity() const {
  return _declaration.params().size();
}
