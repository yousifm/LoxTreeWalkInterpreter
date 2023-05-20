#pragma once

#include <token.h>

#include <string>

class Lox {
public:
  static void runFile(const std::string &);
  static void runPrompt();
  static void run(const std::string &);
  static void error(size_t, const std::string &);
  static void report(size_t, const std::string &, const std::string &);
  static void print_any(const std::any&);
private:
  static bool hadError;
};
