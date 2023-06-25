#pragma once

#include <lox_class.h>

#include <map>

class Token;

class LoxInstance {
public:
  LoxInstance(LoxClass*);
  
  const LoxType get(Token name) const;

  bool operator==(const LoxInstance& other) const;
private:
  LoxClass* _loxClass;
  std::map<std::string, LoxType> _fields;
};

