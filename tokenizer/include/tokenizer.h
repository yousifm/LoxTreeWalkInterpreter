#pragma once

#include "token_type.h"
#include "token.h"

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
  Token makeToken(TOKEN_TYPE, LoxType = std::monostate());
  
  bool isEnd();
  bool advanceIfMatch(char);
  
  char advance();
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
