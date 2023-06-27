#pragma once

#include <lox_class.h>

#include <map>

class Token;

class LoxInstance {
public:
  LoxInstance(LoxClass*);
  
  const LoxType get(Token);
  
  void set(Token, LoxType);

  bool operator==(const LoxInstance&) const;
private:
  LoxClass* _loxClass;
  std::map<std::string, LoxType> _fields;
};

