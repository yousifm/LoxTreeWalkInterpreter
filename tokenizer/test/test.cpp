#include <gtest/gtest.h>

#include <tokenizer.h>
#include <token_type.h>
#include <file.h>
#include <config.h>

#include <filesystem>
#include <variant>
#include <vector>

TEST(TokenizerTest, CanParseAllTokens) {
  std::filesystem::path filepath = config::TEST_RESOURCE_PATH;
  filepath /= "tokenizer";
  filepath /= "alltokens.txt";

  std::string source = readFile(filepath.string());
  Tokenizer t { source };
  std::vector<Token> tokens = t.getTokens(); 
  std::vector<Token> expected {
    Token{TOKEN_TYPE::LEFT_PAREN, "("},
    Token{TOKEN_TYPE::RIGHT_PAREN, ")"},
    Token{TOKEN_TYPE::LEFT_BRACE, "{"},
    Token{TOKEN_TYPE::RIGHT_BRACE, "}"},
    Token{TOKEN_TYPE::COMMA, ","},
    Token{TOKEN_TYPE::DOT, "."},
    Token{TOKEN_TYPE::MINUS, "-"},
    Token{TOKEN_TYPE::PLUS, "+"},
    Token{TOKEN_TYPE::SEMICOLON, ";"},
    Token{TOKEN_TYPE::SLASH, "/"},
    Token{TOKEN_TYPE::STAR, "*"},
    Token{TOKEN_TYPE::BANG, "!"},
    Token{TOKEN_TYPE::BANG_EQUAL, "!="},
    Token{TOKEN_TYPE::EQUAL, "="},
    Token{TOKEN_TYPE::EQUAL_EQUAL, "=="},
    Token{TOKEN_TYPE::GREATER, ">"},
    Token{TOKEN_TYPE::GREATER_EQUAL, ">="},
    Token{TOKEN_TYPE::LESS, "<"},
    Token{TOKEN_TYPE::LESS_EQUAL, "<="},
    Token{TOKEN_TYPE::IDENTIFIER, "identifier"},
    Token{TOKEN_TYPE::STRING, "\"string\"", "string"},
    Token{TOKEN_TYPE::NUMBER, "123", (double) 123},
    Token{TOKEN_TYPE::AND, "and"},
    Token{TOKEN_TYPE::CLASS, "class"},
    Token{TOKEN_TYPE::ELSE, "else"},
    Token{TOKEN_TYPE::FALSE, "false", false},
    Token{TOKEN_TYPE::FUN, "fun"},
    Token{TOKEN_TYPE::FOR, "for"},
    Token{TOKEN_TYPE::IF, "if"},
    Token{TOKEN_TYPE::NIL, "nil"},
    Token{TOKEN_TYPE::OR, "or"},
    Token{TOKEN_TYPE::PRINT, "print"},
    Token{TOKEN_TYPE::RETURN, "return"},
    Token{TOKEN_TYPE::SUPER, "super"},
    Token{TOKEN_TYPE::THIS, "this"},
    Token{TOKEN_TYPE::TRUE, "true", true},
    Token{TOKEN_TYPE::VAR, "var"},
    Token{TOKEN_TYPE::WHILE, "while"},
    Token{TOKEN_TYPE::END_OF_FILE, ""},
  };
  
  EXPECT_EQ(tokens, expected);
}
