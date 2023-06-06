#include "expr.h"
#include <file.h>
#include <interpreter.h>
#include <lox.h>
#include <parser.h>
#include <printer_visitor.h>
#include <runtime_error.h>
#include <tokenizer.h>

#include <iostream>
#include <vector>

void Lox::runFile(const std::string &path) {
  std::string content = readFile(path);
  run(content);
}

void Lox::runPrompt() {
  std::string line;

  for (;;) {
    line.clear();

    std::cout << "> ";
    std::getline(std::cin, line);

    run(line);
  }
}

void Lox::run(const std::string &source) {
  hadError = false;

  Tokenizer tokenizer{source};
  std::vector<Token> tokens = tokenizer.getTokens();

  Parser parser{std::move(tokens)};
  const std::vector<Stmt::Stmt *> statements = parser.parse();

  if (hadError)
    return;

  interpreter.interpret(statements);
}

void Lox::error(size_t line, const std::string &message) {
  report(line, "", message);
}

void Lox::report(size_t line, const std::string &location,
                 const std::string &message) {
  hadError = true;

  std::cerr << "[line " << line << "] Error " << location << ": " << message
            << std::endl;
}

void Lox::runtime_error(RuntimeError err) {
  std::cout << "Runtime Error. Operator " << err._token << ": " << err.what()
            << std::endl;

  hadError = true;
}

bool Lox::hadError = false;
Interpreter Lox::interpreter{};
