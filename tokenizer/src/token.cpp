#include <string>
#include <token.h>

#include <iostream>
#include <variant>

Token::Token(TOKEN_TYPE type, const std::string& lexeme)
  : _type(type), _lexeme(lexeme), _literal(std::monostate()), _line(0) {}

Token::Token(TOKEN_TYPE type, const std::string& lexeme, literal_variant literal)
: _type(type), _lexeme(lexeme), _literal(literal), _line(0) {}

Token::Token(TOKEN_TYPE type, const std::string& lexeme, literal_variant literal, size_t line)
  : _type(type), _lexeme(lexeme), _literal(literal), _line(line) {}

bool Token::operator==(const Token& other) const {
  return (_type == other._type) && (_lexeme == other._lexeme) && (_literal == other._literal);
}

struct variant_print {
  std::string operator()(const std::string& x) const {return x;}
  std::string operator()(double x) const { return std::to_string(x);}
  std::string operator()(const std::monostate& x) const {return "";}
};

std::ostream& operator<<(std::ostream& outs, const Token& token)
{
  outs << token._type << " " << token._lexeme << " ";
  outs << std::visit(variant_print(), token._literal);

  return outs;
}
