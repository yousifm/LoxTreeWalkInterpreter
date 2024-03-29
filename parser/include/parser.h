#pragma once

#include <expr.h>
#include <stmt.h>
#include <token.h>
#include <vector>

class Parser {
public:
  struct Exception : public std::runtime_error {
    Exception(const std::string &message) : runtime_error(message.c_str()) {}
  };

  Parser(std::vector<Token>);
  Parser(std::vector<Token>, bool);

  std::vector<Stmt::Stmt *> parse();

private:
  Stmt::Stmt *declaration();
  Stmt::Stmt *varDeclaration();
  Stmt::Stmt *funDeclaration();
  Stmt::Stmt *classDeclaration();
  Stmt::Stmt *statement();
  Stmt::Stmt *printStatement();
  Stmt::Stmt *expressionStatement();
  std::vector<const Stmt::Stmt *> block();
  Stmt::Stmt *ifStatement();
  Stmt::Stmt *whileStatement();
  Stmt::Stmt *forStatement();
  Stmt::Stmt *returnStatement();

  Expr::Expr *expression();
  Expr::Expr *assignment();
  Expr::Expr *ternary();
  Expr::Expr *logic();
  Expr::Expr *equality();
  Expr::Expr *comparison();
  Expr::Expr *term();
  Expr::Expr *factor();
  Expr::Expr *unary();
  Expr::Expr *call();
  Expr::Expr *primary();

  bool advanceIfMatch(std::initializer_list<TOKEN_TYPE>);
  bool check(TOKEN_TYPE);
  bool isEnd();

  Token peek();
  Token previous();

  void advance();
  void synchronize();
  Token consume(TOKEN_TYPE, const std::string &);

  Exception error(Token, const std::string &);

  int _current = 0;
  bool _is_repl = false;
  std::vector<Token> _tokens;
};
