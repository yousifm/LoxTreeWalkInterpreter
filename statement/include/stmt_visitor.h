#pragma once

#include "stmt.h"

namespace Stmt {

class StmtVisitor {
public:
  virtual void visitExprStmt(const ExprStmt *) = 0;
  virtual void visitPrintStmt(const PrintStmt *) = 0;
  virtual void visitVarStmt(const VarStmt *) = 0;
  virtual void visitBlock(const Block *) = 0;
  virtual void visitIfStmt(const IfStmt *) = 0;
};

} // namespace Stmt
