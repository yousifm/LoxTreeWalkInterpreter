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

Stmt::Stmt *Parser::declaration() {
  try {
    if (advanceIfMatch({CLASS}))
      return classDeclaration();
    else if (advanceIfMatch({VAR}))
      return varDeclaration();
    else if (advanceIfMatch({FUN}))
      return funDeclaration();

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

Stmt::Stmt *Parser::funDeclaration() {
  Token name = consume(IDENTIFIER, "Expect function name.");
  consume(LEFT_PAREN, "Expect '(' after function name.");
  std::vector<Token> params;
  if (!check(RIGHT_PAREN)) {
    do {
      if (params.size() >= 255) {
        error(peek(), "Can not have more than 255 parameters.");
      }

      params.push_back(consume(IDENTIFIER, "Expect parameter name."));
    } while (advanceIfMatch({COMMA}));
  }
  consume(RIGHT_PAREN, "Expect ')' after parameters.");

  consume(LEFT_BRACE, "Expect '{' before function body.");
  std::vector<const Stmt::Stmt *> body = block();
  return new Stmt::FunctionStmt(name, params, body);
}

Stmt::Stmt *Parser::classDeclaration() {
  Token name = consume(IDENTIFIER, "Expected name after class keyword.");
  consume (LEFT_BRACE, "Expect '{' before class body.");

  std::vector<Stmt::FunctionStmt*> methods;
  while (!check(RIGHT_BRACE) && !isEnd()) {
    methods.push_back(dynamic_cast<Stmt::FunctionStmt*>(funDeclaration()));
  }

  consume(RIGHT_BRACE, "Expect '}' after class body.");

  return new Stmt::ClassStmt(name, methods);
}

Stmt::Stmt *Parser::statement() {
  if (advanceIfMatch({PRINT}))
    return printStatement();
  if (advanceIfMatch({RETURN}))
    return returnStatement();
  if (advanceIfMatch({LEFT_BRACE}))
    return new Stmt::Block(block());
  if (advanceIfMatch({IF}))
    return ifStatement();
  if (advanceIfMatch({WHILE}))
    return whileStatement();
  if (advanceIfMatch({FOR}))
    return forStatement();

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

Stmt::Stmt *Parser::ifStatement() {
  consume(LEFT_PAREN, "Expect '(' after if.");
  Expr::Expr *condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after expr.");

  Stmt::Stmt *thenBranch = statement();
  Stmt::Stmt *elseBranch = nullptr;
  if (advanceIfMatch({ELSE})) {
    elseBranch = statement();
  }

  return new Stmt::IfStmt(condition, thenBranch, elseBranch);
}

Stmt::Stmt *Parser::whileStatement() {
  consume(LEFT_PAREN, "Expect '(' after while.");
  Expr::Expr *condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after expr.");

  Stmt::Stmt *body = statement();
  return new Stmt::WhileStmt(condition, body);
}

Stmt::Stmt *Parser::forStatement() {
  consume(LEFT_PAREN, "Expect '(' after for.");
  Stmt::Stmt *init = nullptr;
  if (!advanceIfMatch({SEMICOLON})) {
    if (advanceIfMatch({VAR}))
      init = varDeclaration();
    else
      init = expressionStatement();
  }
  Expr::Expr *condition = nullptr;
  if (!advanceIfMatch({SEMICOLON})) {
    condition = expression();
    consume(SEMICOLON, "Expect ';'.");
  }
  Expr::Expr *after = nullptr;
  if (!advanceIfMatch({RIGHT_PAREN})) {
    after = expression();
    consume(RIGHT_PAREN, "Expect ')'.");
  }

  Stmt::Stmt *body = statement();

  return new Stmt::ForStmt(init, condition, after, body);
}

Stmt::Stmt *Parser::returnStatement() {
  Token ret = previous();
  Expr::Expr* value = nullptr;
  if (!check(SEMICOLON)) {
    value = expression();
  }

  consume(SEMICOLON, "Expected semicolon after return statement.");
  return new Stmt::ReturnStmt(ret, value);
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
  Expr::Expr *expr = logic();

  if (advanceIfMatch({QUESTION_MARK})) {
    Expr::Expr *first = expression();

    consume(SEMICOLON, "Expected ':' for ternary expression");

    Expr::Expr *second = expression();
    expr = new Expr::TernaryExpr(expr, first, second);
  }

  return expr;
}

Expr::Expr *Parser::logic() {
  Expr::Expr *expr = equality();

  if (advanceIfMatch({OR, AND})) {
    Token op = previous();
    Expr::Expr *second = equality();

    expr = new Expr::LogicExpr(op, expr, second);
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

  return call();
}

Expr::Expr *Parser::call() {
  Expr::Expr *expr = primary();

  while (true) {
    if (advanceIfMatch({LEFT_PAREN})) {
      std::vector<Expr::Expr *> arguments;
      if (!check(RIGHT_PAREN)) {
        do {
          if (arguments.size() >= 255)
            error(peek(), "Can't have more than 255 arguments.");
          arguments.push_back(expression());
        } while (advanceIfMatch({COMMA}));
      }

      Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");
      expr = new Expr::CallExpr(expr, paren, arguments);
    } else {
      break;
    }
  }

  return expr;
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
