#include <interpreter.h>
#include <file.h>
#include <tokenizer.h>

#include <vector>
#include <iostream>

void runFile(const std::string& path) {
  std::string content = readFile(path);
  run(content);
}

void runPrompt() {
  std::string line;

  for (;;) {
    line.clear();
    
    std::cout << "> ";
    std::getline(std::cin, line);
    
    run(line);
  }
}

void run(const std::string& source) {
  Tokenizer tokenizer{source};
  std::vector<Token> tokens = tokenizer.getTokens();

  for (Token token : tokens) {
    std::cout << token << std::endl;
  }
}

void error(size_t line, const std::string& message) {
  report(line, "", message);
}

void report(size_t line, const std::string& location, const std::string& message) {
  std::cerr << "[line " << line << "] Error " << location << ": " << message << std::endl;
}
