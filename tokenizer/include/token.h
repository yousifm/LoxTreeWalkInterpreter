#pragma once

#include "token_type.h"

#include <iostream>
#include <variant>
#include <string>

class Token {
public:
  struct variant_print {
    std::string operator()(const std::string& x) const {return x;}
    std::string operator()(double x) const { return std::to_string(x);}
    std::string operator()(const std::monostate& x) const {return "nil";}
  };

  typedef std::variant<std::monostate, std::string, double> literal_variant;
  
  Token(TOKEN_TYPE, std::string );
  Token(TOKEN_TYPE, std::string , literal_variant);
  Token(TOKEN_TYPE, std::string , literal_variant, size_t);
  
  TOKEN_TYPE type()    const      {return _type;}
  std::string lexeme() const      {return _lexeme;}
  literal_variant literal() const {return _literal;}

  bool operator==(const Token&) const;
  friend std::ostream& operator<<(std::ostream&, const Token&);
private:
  TOKEN_TYPE _type;
  std::string _lexeme;
  literal_variant _literal;
  size_t _line;
};
