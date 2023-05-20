#include <expr.h>
#include <token.h>

#include <vector>

class Parser {
public:
  struct Exception : public std::runtime_error {
    Exception(const std::string &message) : runtime_error(message.c_str()) {}
  };

  Parser(std::vector<Token>);

  Expr::expr_ptr parse();

private:
  Expr::expr_ptr expression();
  Expr::expr_ptr tertiary();
  Expr::expr_ptr equality();
  Expr::expr_ptr comparison();
  Expr::expr_ptr term();
  Expr::expr_ptr factor();
  Expr::expr_ptr unary();
  Expr::expr_ptr primary();

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
