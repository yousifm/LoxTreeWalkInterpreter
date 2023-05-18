#include <printer_visitor.h>

void PrinterVisitor::visitBinary(std::shared_ptr<const Expr::BinaryExpr> expr) {
  parenthesize(expr->op().lexeme(), {expr->left(), expr->right()});
}

void PrinterVisitor::visitLiteral(
    std::shared_ptr<const Expr::LiteralExpr> expr) {
  _builder << std::visit(Token::variant_print(), expr->value());
}

void PrinterVisitor::visitGrouping(
    std::shared_ptr<const Expr::GroupingExpr> expr) {
  parenthesize("group", {expr->expr()});
}

void PrinterVisitor::visitUnary(std::shared_ptr<const Expr::UnaryExpr> expr) {
  parenthesize(expr->op().lexeme(), {expr->right()});
}

std::string PrinterVisitor::getOutput() { return _builder.str(); }

void PrinterVisitor::print(const std::shared_ptr<Expr::Expr> &expr) {
  expr->accept(this);
  std::cout << getOutput() << std::endl;
}

void PrinterVisitor::parenthesize(
    const std::string &name,
    const std::vector<std::shared_ptr<Expr::Expr>> &exprs) {
  _builder << "(" << name;
  for (const auto &expr : exprs) {
    _builder << " ";

    expr->accept(this);
  }
  _builder << ")";
}
