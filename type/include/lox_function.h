#pragma once

#include <lox_callable.h>
#include <stmt.h>

class LoxFunction : public LoxCallable {
public:
  LoxFunction(const Stmt::FunctionStmt); 

  std::any call(Interpreter*, const std::vector<std::any>&) const override;

  size_t arity() const override; 
private:
  Stmt::FunctionStmt _declaration;
};
