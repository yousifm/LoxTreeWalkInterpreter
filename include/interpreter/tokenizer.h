#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "token_type.h"
#include <token.h>

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <unordered_map>

class Tokenizer {
public:
  Tokenizer(const std::string&);

  std::vector<Token> getTokens();
  
  static const std::unordered_map<std::string, TOKEN_TYPE> _keywords;
private:
  std::optional<Token> getToken();
  Token makeToken(TOKEN_TYPE, Token::literal_variant literal_variant = std::monostate());
  
  bool isEnd();
  bool isNext(char);
  
  char next();
  char peek();
  char peekNext();
  
  void skip(size_t);

  std::optional<Token> handleString();
  std::optional<Token> handleNumber();
  std::optional<Token> handleIdentifier();
  void handleMultilineComment();

  std::string _source;
  size_t _start = 0;
  size_t _current = 0;
  size_t _line = 1;
};

#endif
