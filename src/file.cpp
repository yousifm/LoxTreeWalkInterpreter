#include <fstream>
#include <sstream>

#include <file.h>

std::string readFile(const std::string& path) {
  std::ifstream input(path);
  
  std::stringstream buffer;
  buffer << input.rdbuf();
  
  return buffer.str();
}

