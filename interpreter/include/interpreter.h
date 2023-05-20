#include <expression_visitor.h>

class Interpreter : public Expr::ExprVisitor {
public:
  void visitGrouping(const Expr::GroupingExpr *) override;
  void visitUnary(const Expr::UnaryExpr *) override;
  void visitBinary(const Expr::BinaryExpr *) override;
  void visitLiteral(const Expr::LiteralExpr *) override;
  void visitTernary(const Expr::TernaryExpr *) override;
  
  std::any eval(const Expr::Expr*);
private:
  void evalutate(const Expr::Expr *);
  void enforceDouble(Token, const std::any &);

  bool isTruthy(const std::any &);
  bool isEqual(const std::any &, const std::any &);

  template <typename T> bool isOfType(const std::any &);

  std::any _value;
};
