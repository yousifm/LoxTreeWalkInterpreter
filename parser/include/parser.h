#include <expr.h>
#include <token.h>

#include <vector>

class Parser {
public:
  struct Exception : public std::runtime_error {
    Exception(const std::string &message) : runtime_error(message.c_str()) {}
  };

  Parser(std::vector<Token>);

  Expr::Expr* parse();

private:
  Expr::Expr* expression();
  Expr::Expr* ternary();
  Expr::Expr* equality();
  Expr::Expr* comparison();
  Expr::Expr* term();
  Expr::Expr* factor();
  Expr::Expr* unary();
  Expr::Expr* primary();

  bool advanceIfMatch(std::initializer_list<TOKEN_TYPE>);
  bool check(TOKEN_TYPE);
  bool isEnd();

  Token peek();
  Token previous();

  void advance();
  void consume(TOKEN_TYPE, const std::string &);

  Exception error(Token, const std::string &);

  int _current = 0;
  std::vector<Token> _tokens;
};
