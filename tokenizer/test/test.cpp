#include <gtest/gtest.h>

#include <tokenizer.h>
#include <token_type.h>

#include <variant>
#include <vector>

TEST(TokenizerTest, First) {
  Tokenizer t { "print \"Hello, World!\"" };
  std::vector<Token> tokens = t.getTokens(); 
  std::vector<Token> expected {
    Token{TOKEN_TYPE::PRINT, "print", std::monostate(), 1},
    Token{TOKEN_TYPE::STRING, "\"Hello, World!\"", "Hello, World!", 1},
    Token{TOKEN_TYPE::END_OF_FILE, "", std::monostate(), 1}
  };
  
  EXPECT_EQ(tokens, expected);
}
