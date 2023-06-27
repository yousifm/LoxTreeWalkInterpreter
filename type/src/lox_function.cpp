#include <lox_function.h>
#include <interpreter.h>
#include <return.h>

LoxFunction::LoxFunction(const Stmt::FunctionStmt declaration, std::shared_ptr<Environment> enclosing)
    : _declaration(declaration), _closure(enclosing) {}

LoxFunction::LoxFunction(const LoxFunction& other) : _declaration(other._declaration), _closure(other._closure) {}

LoxType LoxFunction::call(Interpreter *interpreter,
                           const std::vector<LoxType> &args) {
  for (size_t i = 0; i < _declaration.params().size(); i++) {
    _closure.define(_declaration.params().at(i).lexeme(), args.at(i));
  }
  
  try {
    interpreter->executeBlock(std::make_shared<Environment>(std::make_shared<Environment>(_closure)), _declaration.body());
  } catch (Return r) {
    return r.value();
  }

  return LoxType(std::monostate());
}

size_t LoxFunction::arity() const {
  return _declaration.params().size();
}

LoxFunction* LoxFunction::bind(LoxInstance* instance) {
  LoxFunction* func  = new LoxFunction(_declaration, std::make_shared<Environment>(_closure));
  func->_closure.define("this", instance);
  
  return func;
}

