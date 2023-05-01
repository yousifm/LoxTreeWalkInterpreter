#include <cctype>
#include <interpreter.h>
#include <string>
#include <token.h>
#include <token_type.h>
#include <tokenizer.h>

#include <sstream>

std::unordered_map<std::string, TOKEN_TYPE> Tokenizer::_keywords = {
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

bool Tokenizer::isEnd() {
  return _current >= _source.size();
}

std::optional<Token> Tokenizer::getToken() {
  char c = next();
  
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
      tokenopt = nextIs('=') ? makeToken(BANG_EQUAL) : makeToken(BANG);
      break;
    case '=':
      tokenopt = nextIs('=') ? makeToken(EQUAL_EQUAL) : makeToken(EQUAL);
      break;
    case '<':
      tokenopt = nextIs('=') ? makeToken(LESS_EQUAL) : makeToken(LESS);
      break;
    case '>':
      tokenopt = nextIs('=') ? makeToken(GREATER_EQUAL) : makeToken(GREATER);
      break;
    case '/':
      if(nextIs('/')) {
        while(peek() != '\n' && !isEnd()) next();
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

char Tokenizer::next() {
  return _source[_current++];
}

Token Tokenizer::makeToken(TOKEN_TYPE type, Token::literal_variant literal) {
  std::string text = _source.substr(_start, _current - _start);
  return Token{type, text, literal, _line};
}

bool Tokenizer::nextIs(char expected) {
  if (isEnd()) return false;
  if (_source[_current] != expected) return false;

  _current++;
  return true;
}

char Tokenizer::peek() {
  if (isEnd()) return '\0';
  return _source[_current];
}

char Tokenizer::peekNext() {
  if (_current + 1 >= _source.size()) return '\0';

  return _source[_current + 1];
}

std::optional<Token> Tokenizer::handleString() {
  while (peek() != '"' && !isEnd()) {
    if (peek() == '\n') _line++;
    next();
  }

  if (isEnd()) {
    error(_line, "Unterminated string.");
    return std::nullopt;
  }
  
  next();

  std::string literal = _source.substr(_start + 1, _current - _start - 2);

  return makeToken(STRING, literal);
}

std::optional<Token> Tokenizer::handleNumber() {
  while (isdigit(peek())) next();

  if (peek() == '.' && isdigit(peekNext())) {
    next();

    while (isdigit(peek())) next();
  }
  
  std::string substring = _source.substr(_start, _current - _start);

  return makeToken(NUMBER, std::stod(_source.substr(_start, _current - _start)));
}

std::optional<Token> Tokenizer::handleIdentifier() {
  while (isalpha(peek())) next();
  
  std::string substring = _source.substr(_start, _current - _start);
  
  TOKEN_TYPE type = IDENTIFIER;

  if (_keywords.contains(substring)) {
    type = _keywords.at(substring);
  }

  return makeToken(type, substring);
}
