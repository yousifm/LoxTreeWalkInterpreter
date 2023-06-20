#pragma once

#include <lox_class.h>

#include <unordered_map>

class Token;

class LoxInstance {
public:
  LoxInstance(LoxClass*);
  
  const LoxType get(Token name) const;

  bool operator==(const LoxInstance& other) const;
private:
  LoxClass* _loxClass;
  std::unordered_map<std::string, LoxType> _fields;
};

