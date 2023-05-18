#pragma once

#include "expression_visitor.h"
#include <token.h>

#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

class PrinterVisitor : public Expr::ExprVisitor {
public:
  void visitBinary(std::shared_ptr<Expr::BinaryExpr const> expr) override;

  void visitLiteral(std::shared_ptr<Expr::LiteralExpr const> expr) override;

  void visitGrouping(std::shared_ptr<Expr::GroupingExpr const> expr) override;

  void visitUnary(std::shared_ptr<Expr::UnaryExpr const> expr) override;

  std::string getOutput();

  void print(const std::shared_ptr<Expr::Expr> &expr);

private:
  void parenthesize(const std::string &name,
                    const std::vector<std::shared_ptr<Expr::Expr>> &exprs);

  std::stringstream _builder{};
};
