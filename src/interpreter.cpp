#include <interpreter.h>
#include <file.h>

#include <iostream>

void runFile(const std::string& path) {
  std::string content = readFile(path);
}

void runPrompt() {
  std::string line;

  for (;;) {
    line.clear();
    
    std::cout << "> ";
    std::getline(std::cin, line);
    
    std::cout << line << std::endl;
  }
}
