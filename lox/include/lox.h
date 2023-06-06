#pragma once

#include <token.h>
#include <runtime_error.h>
#include <parser.h>
#include <interpreter.h>

#include <string>

class Lox {
public:
  static void runFile(const std::string &);
  static void runPrompt();
  static void run(const std::string &);
  static void error(size_t, const std::string &);
  static void report(size_t, const std::string &, const std::string &);
  static void runtime_error(RuntimeError err);
private:
  static Interpreter interpreter;
  static bool hadError;
};
