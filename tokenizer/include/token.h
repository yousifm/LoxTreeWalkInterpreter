#pragma once

#include "token_type.h"
#include <lox_type.h>

#include <any>
#include <iostream>
#include <string>
#include <variant>

class Token {
public:
  Token(TOKEN_TYPE, std::string);
  Token(TOKEN_TYPE, std::string, LoxType);
  Token(TOKEN_TYPE, std::string, LoxType, size_t);

  TOKEN_TYPE type() const { return _type; }
  std::string lexeme() const { return _lexeme; }
  const LoxType &literal() const { return _literal; }

  size_t line() const { return _line; }
  bool operator==(const Token &) const;
  friend std::ostream &operator<<(std::ostream &, const Token &);

private:
  TOKEN_TYPE _type;
  std::string _lexeme;
  LoxType _literal;
  size_t _line;
};
