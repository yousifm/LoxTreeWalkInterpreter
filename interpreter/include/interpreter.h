#pragma once

#include <token.h>

#include <string>

void runFile(const std::string&);

void runPrompt();

void run(const std::string&);

void error(size_t, const std::string&);

void report(size_t, const std::string&, const std::string&);
