#include <exception>

#include <token.h>

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(Token token, std::string message) : std::runtime_error(message.c_str()), _token(token) {} 
private:
  const Token _token;
};
