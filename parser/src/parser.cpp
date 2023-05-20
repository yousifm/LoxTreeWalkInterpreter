#include "parser.h"
#include "expr.h"
#include "lox.h"
#include "token_type.h"

Parser::Parser(std::vector<Token> tokens) : _tokens(std::move(tokens)) {}

Expr::Expr* Parser::parse() {
  try {
    return expression();
  } catch (Parser::Exception e) {
    return new Expr::LiteralExpr(nullptr);
  }
}

Expr::Expr* Parser::expression() { return ternary(); }

Expr::Expr* Parser::ternary() {
  Expr::Expr* expr = comparison();

  if (advanceIfMatch({QUESTION_MARK})) {
    Expr::Expr* first = expression();
    
    consume(SEMICOLON, "Expected semicolon");

    Expr::Expr* second = expression();
    expr = new Expr::TernaryExpr(expr, first, second);
  }

  return expr;
}

Expr::Expr* Parser::equality() {
  Expr::Expr* expr = comparison();

  while (advanceIfMatch({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op = previous();

    Expr::Expr* right = comparison();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr* Parser::comparison() {
  Expr::Expr* expr = term();

  while (advanceIfMatch({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op = previous();
    Expr::Expr* right = term();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr* Parser::term() {
  Expr::Expr* expr = factor();

  while (advanceIfMatch({MINUS, PLUS})) {
    Token op = previous();
    Expr::Expr* right = factor();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr* Parser::factor() {
  Expr::Expr* expr = unary();

  while (advanceIfMatch({SLASH, STAR})) {
    Token op = previous();
    Expr::Expr* right = unary();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr* Parser::unary() {
  while (advanceIfMatch({MINUS, BANG})) {
    Token op = previous();
    Expr::Expr* expr = unary();
    return new Expr::UnaryExpr(op, expr);
  }

  return primary();
}

Expr::Expr* Parser::primary() {
  if (advanceIfMatch({FALSE}))
    return new Expr::LiteralExpr(false);
  if (advanceIfMatch({TRUE}))
    return new Expr::LiteralExpr(true);
  if (advanceIfMatch({NIL}))
    return new Expr::LiteralExpr(std::monostate());
  if (advanceIfMatch({NUMBER, STRING})) {
    return new Expr::LiteralExpr(previous().literal());
  }

  if (advanceIfMatch({LEFT_PAREN})) {
    Expr::Expr* expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");

    return new Expr::GroupingExpr(expr);
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
    Lox::report(token.line(), " at end", message);
  } else {
    Lox::report(token.line(), "at '" + token.lexeme() + "'", message);
  }

  return Parser::Exception{message};
}
