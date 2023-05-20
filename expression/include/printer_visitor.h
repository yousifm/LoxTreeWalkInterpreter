#pragma once

#include "expression_visitor.h"
#include <token.h>

#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

class PrinterVisitor : public Expr::ExprVisitor {
public:
  void visitBinary(const Expr::BinaryExpr* expr) override;

  void visitLiteral(const Expr::LiteralExpr* expr) override;

  void visitGrouping(const Expr::GroupingExpr* expr) override;

  void visitUnary(const Expr::UnaryExpr* expr) override;

  void visitTernary(const Expr::TernaryExpr* expr) override;

  std::string print(const Expr::Expr *expr);

private:
  void parenthesize(const std::string &name,
                    const std::vector<const Expr::Expr*> &exprs);

  std::string getOutput();

  std::stringstream _builder{};
};
