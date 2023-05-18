#include "expr.h"
#include <file.h>
#include <interpreter.h>
#include <parser.h>
#include <printer_visitor.h>
#include <tokenizer.h>

#include <iostream>
#include <vector>

void Interpretter::runFile(const std::string &path) {
  std::string content = readFile(path);
  run(content);
}

void Interpretter::runPrompt() {
  std::string line;

  for (;;) {
    line.clear();

    std::cout << "> ";
    std::getline(std::cin, line);

    run(line);
  }
}

void Interpretter::run(const std::string &source) {
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

void Interpretter::error(size_t line, const std::string &message) {
  report(line, "", message);
}

void Interpretter::report(size_t line, const std::string &location,
                          const std::string &message) {
  hadError = true;

  std::cerr << "[line " << line << "] Error " << location << ": " << message
            << std::endl;
}

bool Interpretter::hadError = false;
