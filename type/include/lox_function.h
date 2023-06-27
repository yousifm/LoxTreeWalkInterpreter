#pragma once

#include <environment.h>
#include <lox_callable.h>
#include <stmt.h>

class LoxFunction : public LoxCallable {
public:
  LoxFunction(const Stmt::FunctionStmt, std::shared_ptr<Environment>);
  LoxFunction(const LoxFunction&);

  LoxType call(Interpreter *, const std::vector<LoxType> &) override;

  size_t arity() const override;

private:
  Stmt::FunctionStmt _declaration;
  Environment _closure;
};
