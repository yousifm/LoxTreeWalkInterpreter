#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "token_type.h"

#include <iostream>
#include <variant>
#include <string>

class Token {
public:
  typedef std::variant<std::monostate, std::string, double> literal_variant;
  
  Token(TOKEN_TYPE, const std::string&);
  Token(TOKEN_TYPE, const std::string&, const literal_variant);
  Token(TOKEN_TYPE, const std::string&, const literal_variant, size_t);

  bool operator==(const Token&) const;
  friend std::ostream& operator<<(std::ostream&, const Token&);
private:
  TOKEN_TYPE _type;
  std::string _lexeme;
  literal_variant _literal;
  size_t _line;
};

#endif
