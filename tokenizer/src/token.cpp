#include <string>
#include <token.h>

#include <iostream>
#include <utility>
#include <variant>

Token::Token(TOKEN_TYPE type, std::string  lexeme)
  : _type(type), _lexeme(std::move(lexeme)), _literal(), _line(0) {}

Token::Token(TOKEN_TYPE type, std::string  lexeme, LoxType literal)
: _type(type), _lexeme(std::move(lexeme)), _literal(std::move(literal)), _line(0) {}

Token::Token(TOKEN_TYPE type, std::string  lexeme, LoxType literal, size_t line)
  : _type(type), _lexeme(std::move(lexeme)), _literal(std::move(literal)), _line(line) {}

bool Token::operator==(const Token& other) const {
  return (_type == other._type) && (_lexeme == other._lexeme) && (_literal == other._literal);
}

std::ostream& operator<<(std::ostream& outs, const Token& token)
{
  outs << token._lexeme << " ";
  if (!token._literal.empty())
    outs << token._literal;

  return outs;
}
