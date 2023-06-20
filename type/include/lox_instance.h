#pragma once

#include <lox_class.h>

class LoxInstance {
public:
  LoxInstance(LoxClass* loxClass) : _loxClass(loxClass) {}

  bool operator==(const LoxInstance& other) const {
    return false;
  }
private:
  LoxClass* _loxClass;
};

