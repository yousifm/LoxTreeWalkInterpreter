#include "parser.h"
#include "expr.h"
#include "lox.h"
#include "token_type.h"

Parser::Parser(std::vector<Token> tokens) : _tokens(std::move(tokens)) {}

std::vector<Stmt::Stmt *> Parser::parse() {
  std::vector<Stmt::Stmt *> statements;

  try {
    while (!isEnd()) {
      statements.push_back(declaration());
    }
  } catch (Exception) {
  }

  return statements;
}

Stmt::Stmt *Parser::ifStatement() {
  consume(LEFT_PAREN, "Expect '(' after if.");
  Expr::Expr *condition = expression();
  consume(RIGHT_PAREN, "Expect ')'.");

  Stmt::Stmt *thenBranch = statement();
  Stmt::Stmt *elseBranch = nullptr;
  if (advanceIfMatch({ELSE})) {
    elseBranch = statement();
  }

  return new Stmt::IfStmt(condition, thenBranch, elseBranch);  
}

Stmt::Stmt *Parser::declaration() {
  try {
    if (advanceIfMatch({VAR}))
      return varDeclaration();

    return statement();
  } catch (Exception error) {
    synchronize();
    return nullptr;
  }
}

Stmt::Stmt *Parser::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name.");

  Expr::Expr *initializer = nullptr;
  if (advanceIfMatch({EQUAL})) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  return new Stmt::VarStmt(name, initializer);
}

Stmt::Stmt *Parser::statement() {
  if (advanceIfMatch({PRINT}))
    return printStatement();
  if (advanceIfMatch({LEFT_BRACE}))
    return new Stmt::Block(block());
  if (advanceIfMatch({IF}))
    return ifStatement();

  return expressionStatement();
}

Stmt::Stmt *Parser::printStatement() {
  Expr::Expr *expr = expression();
  consume(SEMICOLON, "Expected ';' after expression.");
  return new Stmt::PrintStmt(expr);
}

Stmt::Stmt *Parser::expressionStatement() {
  Expr::Expr *expr = expression();
  consume(SEMICOLON, "Expected ';' after expression.");
  return new Stmt::ExprStmt(expr);
}

std::vector<const Stmt::Stmt *> Parser::block() {
  std::vector<const Stmt::Stmt *> statements;

  while (!check(RIGHT_BRACE) && !isEnd()) {
    statements.push_back(declaration());
  }

  consume(RIGHT_BRACE, "Expected '}' after block.");
  return statements;
}

Expr::Expr *Parser::expression() { return assignment(); }

Expr::Expr *Parser::assignment() {
  Expr::Expr *expr = ternary();

  if (advanceIfMatch({EQUAL})) {
    Token equals = previous();
    Expr::Expr *value = assignment();

    try {
      Token name = dynamic_cast<Expr::VariableExpr *>(expr)->name();

      return new Expr::AssignExpr(name, value);
    } catch (std::exception e) {
      error(equals, "Invalid assignment target.");
    }
  }

  return expr;
}

Expr::Expr *Parser::ternary() {
  Expr::Expr *expr = equality();

  if (advanceIfMatch({QUESTION_MARK})) {
    Expr::Expr *first = expression();

    consume(SEMICOLON, "Expected semicolon");

    Expr::Expr *second = expression();
    expr = new Expr::TernaryExpr(expr, first, second);
  }

  return expr;
}

Expr::Expr *Parser::equality() {
  Expr::Expr *expr = comparison();

  while (advanceIfMatch({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op = previous();

    Expr::Expr *right = comparison();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr *Parser::comparison() {
  Expr::Expr *expr = term();

  while (advanceIfMatch({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op = previous();
    Expr::Expr *right = term();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr *Parser::term() {
  Expr::Expr *expr = factor();

  while (advanceIfMatch({MINUS, PLUS})) {
    Token op = previous();
    Expr::Expr *right = factor();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr *Parser::factor() {
  Expr::Expr *expr = unary();

  while (advanceIfMatch({SLASH, STAR})) {
    Token op = previous();
    Expr::Expr *right = unary();
    expr = new Expr::BinaryExpr(expr, op, right);
  }

  return expr;
}

Expr::Expr *Parser::unary() {
  while (advanceIfMatch({MINUS, BANG})) {
    Token op = previous();
    Expr::Expr *expr = unary();
    return new Expr::UnaryExpr(op, expr);
  }

  return primary();
}

Expr::Expr *Parser::primary() {
  if (advanceIfMatch({FALSE}))
    return new Expr::LiteralExpr(false);
  if (advanceIfMatch({TRUE}))
    return new Expr::LiteralExpr(true);
  if (advanceIfMatch({NIL}))
    return new Expr::LiteralExpr(std::monostate());
  if (advanceIfMatch({NUMBER, STRING})) {
    return new Expr::LiteralExpr(previous().literal());
  }
  if (advanceIfMatch({IDENTIFIER})) {
    return new Expr::VariableExpr(previous());
  }
  if (advanceIfMatch({LEFT_PAREN})) {
    Expr::Expr *expr = expression();
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

Token Parser::consume(TOKEN_TYPE type, const std::string &error_message) {
  if (check(type)) {
    Token ret = peek();
    advance();
    return ret;
  }

  throw error(peek(), error_message);
}

void Parser::synchronize() {
  advance();

  while (!isEnd()) {
    if (previous().type() == SEMICOLON)
      return;

    switch (peek().type()) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    default:
      continue;
    }

    advance();
  }
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
