#include "parser.h"
#include "expr.h"
#include "interpreter.h"
#include "token_type.h"

Parser::Parser(std::vector<Token> tokens) : _tokens(std::move(tokens)) {}

Expr::expr_ptr Parser::parse() {
  try {
    return expression();
  } catch (Parser::Exception e) {
    return Expr::expr_ptr(nullptr);
  }
}

Expr::expr_ptr Parser::expression() { return ternary(); }

Expr::expr_ptr Parser::ternary() {
  Expr::expr_ptr expr = comparison();

  if (advanceIfMatch({QUESTION_MARK})) {
    Expr::expr_ptr first = expression();
    if (advanceIfMatch({SEMICOLON})) {
      Expr::expr_ptr second = expression();
      expr = std::make_shared<Expr::TernaryExpr>(
          Expr::TernaryExpr(expr, first, second));
    } else {
      throw error(peek(), "Expected semicolon");
    }
  }

  return expr;
}

Expr::expr_ptr Parser::equality() {
  Expr::expr_ptr expr = comparison();

  while (advanceIfMatch({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op = previous();

    Expr::expr_ptr right = comparison();
    expr =
        std::make_shared<Expr::BinaryExpr>(Expr::BinaryExpr(expr, op, right));
  }

  return expr;
}

Expr::expr_ptr Parser::comparison() {
  Expr::expr_ptr expr = term();

  while (advanceIfMatch({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op = previous();
    Expr::expr_ptr right = term();
    expr = std::make_shared<Expr::BinaryExpr>(expr, op, right);
  }

  return expr;
}

Expr::expr_ptr Parser::term() {
  Expr::expr_ptr expr = factor();

  while (advanceIfMatch({MINUS, PLUS})) {
    Token op = previous();
    Expr::expr_ptr right = factor();
    expr = std::make_shared<Expr::BinaryExpr>(expr, op, right);
  }

  return expr;
}

Expr::expr_ptr Parser::factor() {
  Expr::expr_ptr expr = unary();

  while (advanceIfMatch({SLASH, STAR})) {
    Token op = previous();
    Expr::expr_ptr right = unary();
    expr = std::make_shared<Expr::BinaryExpr>(expr, op, right);
  }

  return expr;
}

Expr::expr_ptr Parser::unary() {
  while (advanceIfMatch({MINUS, BANG})) {
    Token op = previous();
    Expr::expr_ptr expr = unary();
    return std::make_shared<Expr::UnaryExpr>(op, expr);
  }

  return primary();
}

Expr::expr_ptr Parser::primary() {
  if (advanceIfMatch({FALSE}))
    return std::make_shared<Expr::LiteralExpr>(false);
  if (advanceIfMatch({TRUE}))
    return std::make_shared<Expr::LiteralExpr>(true);
  if (advanceIfMatch({NIL}))
    return std::make_shared<Expr::LiteralExpr>(std::monostate());
  if (advanceIfMatch({NUMBER, STRING})) {
    return std::make_shared<Expr::LiteralExpr>(previous().literal());
  }

  if (advanceIfMatch({LEFT_PAREN})) {
    Expr::expr_ptr expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");

    return std::make_shared<Expr::GroupingExpr>(expr);
  }

  throw error(peek(), "Expected expression");
}

bool Parser::advanceIfMatch(std::initializer_list<TOKEN_TYPE> types) {
  for (TOKEN_TYPE type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::check(TOKEN_TYPE type) {
  if (isEnd())
    return false;

  return _tokens[_current].type() == type;
}

bool Parser::isEnd() { return peek().type() == END_OF_FILE; }

Token Parser::peek() { return _tokens[_current]; }

Token Parser::previous() { return _tokens[_current - 1]; }

void Parser::consume(TOKEN_TYPE type, const std::string &error_message) {
  if (check(type)) {
    return advance();
  }

  throw error(peek(), error_message);
}

void Parser::advance() { _current++; }

Parser::Exception Parser::error(Token token, const std::string &message) {
  if (token.type() == TOKEN_TYPE::END_OF_FILE) {
    Interpretter::report(token.line(), " at end", message);
  } else {
    Interpretter::report(token.line(), "at '" + token.lexeme() + "'", message);
  }

  return Parser::Exception{message};
}
