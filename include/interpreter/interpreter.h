#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>

void runFile(const std::string&);

void runPrompt();

void run(const std::string&);

void error(int, const std::string&);
void report(int, const std::string&, const std::string&);

#endif
