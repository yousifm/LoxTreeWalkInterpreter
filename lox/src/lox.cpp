#include "expr.h"
#include <file.h>
#include <lox.h>
#include <parser.h>
#include <printer_visitor.h>
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

  Parser parser{tokens};
  Expr::expr_ptr expressionTree = parser.parse();

  if (hadError)
    return;

  PrinterVisitor printer;
  printer.print(expressionTree);
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

bool Lox::hadError = false;
