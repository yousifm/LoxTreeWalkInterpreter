#pragma once

#include "stmt.h"

namespace Stmt {

class StmtVisitor {
public:
  virtual void visitExprStmt(const ExprStmt *) = 0;
  virtual void visitPrintStmt(const PrintStmt *) = 0;
  virtual void visitVarStmt(const VarStmt *) = 0;
};

} // namespace Stmt
