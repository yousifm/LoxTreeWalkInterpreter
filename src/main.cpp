#include <iostream>
#include <string>
#include <lox.h>
#include <expr.h>
#include <printer_visitor.h>

int main (int argc, char *argv[]) {
    if (argc > 2) {
    std::cout << "Usage: lox [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    Lox::runFile(argv[1]);
  } else {
    Lox::runPrompt();
  }

  return 0;
}
