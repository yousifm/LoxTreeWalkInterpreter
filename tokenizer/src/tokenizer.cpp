#include <cctype>
#include <lox.h>
#include <string>
#include <token.h>
#include <token_type.h>
#include <tokenizer.h>

#include <sstream>

const std::unordered_map<std::string, TOKEN_TYPE> Tokenizer::_keywords = {
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

Tokenizer::Tokenizer(const std::string &source) : _source(source) {}

std::vector<Token> Tokenizer::getTokens() {
  std::vector<Token> tokens;

  while (!isEnd()) {
    _start = _current;

    std::optional<Token> token = getToken();

    if (token.has_value())
      tokens.emplace_back(std::move(token.value()));
  }

  tokens.emplace_back(END_OF_FILE, "", std::monostate(), _line);

  return tokens;
}

std::optional<Token> Tokenizer::getToken() {
  char c = advance();

  std::optional<Token> tokenopt = std::nullopt;

  switch (c) {
  case '(':
    tokenopt.emplace(makeToken(LEFT_PAREN));
    break;
  case ')':
    tokenopt.emplace(makeToken(RIGHT_PAREN));
    break;
  case '{':
    tokenopt.emplace(makeToken(LEFT_BRACE));
    break;
  case '}':
    tokenopt.emplace(makeToken(RIGHT_BRACE));
    break;
  case ',':
    tokenopt.emplace(makeToken(COMMA));
    break;
  case '.':
    tokenopt.emplace(makeToken(DOT));
    break;
  case '-':
    tokenopt.emplace(makeToken(MINUS));
    break;
  case '+':
    tokenopt.emplace(makeToken(PLUS));
    break;
  case ';':
    tokenopt.emplace(makeToken(SEMICOLON));
    break;
  case '*':
    tokenopt.emplace(makeToken(STAR));
    break;
  case '?':
    tokenopt.emplace(makeToken(QUESTION_MARK));
    break;
  case ':':
    tokenopt.emplace(makeToken(SEMICOLON));
    break;
  case '!':
    tokenopt.emplace(advanceIfMatch('=') ? makeToken(BANG_EQUAL) : makeToken(BANG));
    break;
  case '=':
    tokenopt.emplace(advanceIfMatch('=') ? makeToken(EQUAL_EQUAL) : makeToken(EQUAL));
    break;
  case '<':
    tokenopt.emplace(advanceIfMatch('=') ? makeToken(LESS_EQUAL) : makeToken(LESS));
    break;
  case '>':
    tokenopt.emplace(advanceIfMatch('=') ? makeToken(GREATER_EQUAL) : makeToken(GREATER));
    break;
  case '/':
    if (advanceIfMatch('/')) {
      while (peek() != '\n' && !isEnd())
        advance();
    } else if (advanceIfMatch('*')) {
      handleMultilineComment();
    } else {
      tokenopt.emplace(makeToken(SLASH));
    }
    break;
  case '"':
    tokenopt = handleString();
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    _line++;
    break;
  default:
    if (isdigit(c)) {
      tokenopt = handleNumber();
    } else if (isalpha(c)) {
      tokenopt = handleIdentifier();
    } else {
      std::stringstream s;
      s << "Unexpected character: " << c;

      Lox::error(_line, s.str());
    }
    break;
  }

  return tokenopt;
}

Token Tokenizer::makeToken(TOKEN_TYPE type, LoxType literal) {
  std::string text = _source.substr(_start, _current - _start);
  return Token{type, text, literal, _line};
}

bool Tokenizer::isEnd() { return _current >= _source.size(); }

bool Tokenizer::advanceIfMatch(char expected) {
  if (isEnd())
    return false;
  if (_source[_current] != expected)
    return false;

  _current++;

  return true;
}

char Tokenizer::advance() { return _source[_current++]; }

char Tokenizer::peek() {
  if (isEnd())
    return '\0';
  return _source[_current];
}

char Tokenizer::peekNext() {
  if (_current + 1 >= _source.size())
    return '\0';

  return _source[_current + 1];
}

void Tokenizer::skip(size_t num) { _current += num; }

std::optional<Token> Tokenizer::handleString() {
  while (peek() != '"' && !isEnd()) {
    if (peek() == '\n')
      _line++;
    advance();
  }

  if (isEnd()) {
    Lox::error(_line, "Unterminated string.");
    return std::nullopt;
  }

  advance();

  std::string literal = _source.substr(_start + 1, _current - _start - 2);

  return makeToken(STRING, literal);
}

std::optional<Token> Tokenizer::handleNumber() {
  while (isdigit(peek()))
    advance();

  if (peek() == '.' && isdigit(peekNext())) {
    advance();

    while (isdigit(peek()))
      advance();
  }

  std::string substring = _source.substr(_start, _current - _start);
  double literal = std::stod(substring);

  return makeToken(NUMBER, literal);
}

std::optional<Token> Tokenizer::handleIdentifier() {
  while (isalpha(peek()) || peek() == '_')
    advance();

  std::string substring = _source.substr(_start, _current - _start);

  TOKEN_TYPE type = IDENTIFIER;

  if (_keywords.contains(substring)) {
    type = _keywords.at(substring);
  }

  LoxType literal = std::monostate();

  if (type == FALSE)
    literal = false;
  else if (type == TRUE)
    literal = true;

  return makeToken(type, literal);
}

void Tokenizer::handleMultilineComment() {
  while (!(peek() == '*' && peekNext() == '/') && !isEnd()) {
    if (peek() == '\n')
      _line++;

    advance();
  }

  // Report error if the end is reached without finding teminating characters
  if (isEnd()) {
    Lox::error(_line, "Unterminated multline comment");
  }
  // Skip the terminating characters
  else {
    skip(2);
  }
}
