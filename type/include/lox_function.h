#pragma once

#include <lox_callable.h>
#include <environment.h>
#include <stmt.h>

class LoxFunction : public LoxCallable {
public:
  LoxFunction(const Stmt::FunctionStmt, Environment closure); 

  LoxType call(Interpreter*, const std::vector<LoxType>&) const override;

  size_t arity() const override; 
private:
  Stmt::FunctionStmt _declaration;
  Environment _closure;
};
