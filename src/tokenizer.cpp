#include <cctype>
#include <interpreter.h>
#include <string>
#include <token.h>
#include <token_type.h>
#include <tokenizer.h>

#include <sstream>

const std::unordered_map<std::string, TOKEN_TYPE> Tokenizer::_keywords = {
  {"and",    AND},
  {"class",  CLASS},
  {"else",   ELSE},
  {"false",  FALSE},
  {"for",    FOR},
  {"fun",    FUN},
  {"if",     IF},
  {"nil",    NIL},
  {"or",     OR},
  {"print",  PRINT},
  {"return", RETURN},
  {"super",  SUPER},
  {"this",   THIS},
  {"true",   TRUE},
  {"var",    VAR},
  {"while",  WHILE}
};

Tokenizer::Tokenizer(const std::string& source) : _source(source) {}

std::vector<Token> Tokenizer::getTokens() {
  std::vector<Token> tokens;
  
  while(!isEnd()) {
    _start = _current;

    std::optional<Token> token = getToken();
    
    if (token.has_value())
      tokens.emplace_back(token.value());
  }
  
  tokens.emplace_back(Token{END_OF_FILE, "", "", _line});

  return tokens;
}

std::optional<Token> Tokenizer::getToken() {
  char c = advance();
  
  std::optional<Token> tokenopt = std::nullopt;

  switch (c) {
    case '(': 
      tokenopt = makeToken(RIGHT_PAREN);
      break;
    case ')':
      tokenopt = makeToken(LEFT_PAREN);
      break;
    case '{': 
      tokenopt = makeToken(LEFT_BRACE);
      break;
    case '}': 
      tokenopt = makeToken(RIGHT_BRACE);
      break;
    case ',': 
      tokenopt = makeToken(COMMA);
      break;
    case '.': 
      tokenopt = makeToken(DOT);
      break;
    case '-': 
      tokenopt = makeToken(MINUS);
      break;
    case '+': 
      tokenopt = makeToken(PLUS);
      break;
    case ';': 
      tokenopt = makeToken(SEMICOLON);
      break;
    case '*': 
      tokenopt = makeToken(STAR);
      break;
    case '!':
      tokenopt = isNext('=') ? makeToken(BANG_EQUAL) : makeToken(BANG);
      break;
    case '=':
      tokenopt = isNext('=') ? makeToken(EQUAL_EQUAL) : makeToken(EQUAL);
      break;
    case '<':
      tokenopt = isNext('=') ? makeToken(LESS_EQUAL) : makeToken(LESS);
      break;
    case '>':
      tokenopt = isNext('=') ? makeToken(GREATER_EQUAL) : makeToken(GREATER);
      break;
    case '/':
      if(isNext('/')) {
        while(peek() != '\n' && !isEnd()) advance();
      } else if(isNext('*')) {
        handleMultilineComment();
      } else {
        tokenopt = makeToken(SLASH);
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

        error(_line, s.str());
      }
      break;
  }
   
  return tokenopt; 
}

Token Tokenizer::makeToken(TOKEN_TYPE type, Token::literal_variant literal) {
  std::string text = _source.substr(_start, _current - _start);
  return Token{type, text, literal, _line};
}


bool Tokenizer::isEnd() {
  return _current >= _source.size();
}

bool Tokenizer::isNext(char expected) {
  if (isEnd()) return false;
  if (_source[_current] != expected) return false;

  _current++;
  return true;
}


char Tokenizer::advance() {
  return _source[_current++];
}

char Tokenizer::peek() {
  if (isEnd()) return '\0';
  return _source[_current];
}

char Tokenizer::peekNext() {
  if (_current + 1 >= _source.size()) return '\0';

  return _source[_current + 1];
}

void Tokenizer::skip(size_t num) {
  _current += num;
}


std::optional<Token> Tokenizer::handleString() {
  while (peek() != '"' && !isEnd()) {
    if (peek() == '\n') _line++;
    advance();
  }

  if (isEnd()) {
    error(_line, "Unterminated string.");
    return std::nullopt;
  }
  
  advance();

  std::string literal = _source.substr(_start + 1, _current - _start - 2);

  return makeToken(STRING, literal);
}

std::optional<Token> Tokenizer::handleNumber() {
  while (isdigit(peek())) advance();

  if (peek() == '.' && isdigit(peekNext())) {
    advance();

    while (isdigit(peek())) advance();
  }
  
  std::string substring = _source.substr(_start, _current - _start);

  return makeToken(NUMBER, substring);
}

std::optional<Token> Tokenizer::handleIdentifier() {
  while (isalpha(peek())) advance();
  
  std::string substring = _source.substr(_start, _current - _start);
  
  TOKEN_TYPE type = IDENTIFIER;

  if (_keywords.contains(substring)) {
    type = _keywords.at(substring);
  }

  return makeToken(type);
}

void Tokenizer::handleMultilineComment() {
  char next_char = peek();
  char next_next_char = peekNext();
  
  while (next_char != '*' && next_next_char != '/' && !isEnd()) {
    if (next_char == '\n') _line++;

    next_char = next_next_char;
    advance();
    next_next_char = peekNext();
  }
  
  // Report error if the end is reached without finding teminating characters
  if (isEnd()) {
    error(_line, "Unterminated multline comment");
  }
  // Skip the terminating characters
  else {
    skip(2);
  }
}

