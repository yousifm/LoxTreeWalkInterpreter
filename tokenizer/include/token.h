#pragma once

#include "token_type.h"

#include <iostream>
#include <string>
#include <variant>
#include <any>

class Token {
public:
  struct variant_value_getter {
    std::any operator()(const std::string& x) const { return x; }
    std::any operator()(double x) const { return x; }
    std::any operator()(bool x) const {return x;}
    std::any operator()(const std::monostate& x) const {return x;}
  };

  struct variant_print {
    std::string operator()(const std::string &x) const { return x; }
    std::string operator()(double x) const { return std::to_string(x); }
    std::string operator()(bool x) const { return x ? "true" : "false"; }
    std::string operator()(const std::monostate &x) const { return ""; }
  };

  typedef std::variant<std::monostate, std::string, double, bool>
      literal_variant;

  Token(TOKEN_TYPE, std::string);
  Token(TOKEN_TYPE, std::string, literal_variant);
  Token(TOKEN_TYPE, std::string, literal_variant, size_t);

  TOKEN_TYPE type() const { return _type; }
  std::string lexeme() const { return _lexeme; }
  literal_variant literal() const { return _literal; }
  size_t line() const { return _line; }
  bool operator==(const Token &) const;
  friend std::ostream &operator<<(std::ostream &, const Token &);

private:
  TOKEN_TYPE _type;
  std::string _lexeme;
  literal_variant _literal;
  size_t _line;
};
