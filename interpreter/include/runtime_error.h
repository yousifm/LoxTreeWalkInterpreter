#include <exception>

#include <token.h>

struct RuntimeError : public std::runtime_error {
  RuntimeError(Token token, std::string message) : std::runtime_error(message.c_str()), _token(token) {} 
  const Token _token;
};
