#include <printer_visitor.h>

void PrinterVisitor::visitBinary(const Expr::BinaryExpr* expr) {
  parenthesize(expr->op().lexeme(), {expr->left(), expr->right()});
}

void PrinterVisitor::visitLiteral(
    const Expr::LiteralExpr* expr) {
  _builder << std::visit(Token::variant_print(), expr->value());
}

void PrinterVisitor::visitGrouping(
    const Expr::GroupingExpr* expr) {
  parenthesize("group", {expr->expr()});
}

void PrinterVisitor::visitUnary(const Expr::UnaryExpr* expr) {
  parenthesize(expr->op().lexeme(), {expr->right()});
}

void PrinterVisitor::visitTernary(const Expr::TernaryExpr* expr) {
  parenthesize("tertiary", {expr->condition(), expr->first(), expr->second()});
}

std::string PrinterVisitor::getOutput() { return _builder.str(); }

std::string PrinterVisitor::print(const Expr::Expr* expr) {
  expr->accept(this);
  return getOutput();
}

void PrinterVisitor::parenthesize(
    const std::string &name,
    const std::vector<const Expr::Expr*> &exprs) {
  _builder << "(" << name;
  for (const auto &expr : exprs) {
    _builder << " ";

    expr->accept(this);
  }
  _builder << ")";
}
