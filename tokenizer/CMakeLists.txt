add_library(
  tokenizer
  include/tokenizer.h
  src/tokenizer.cpp
  include/token.h
  src/token.cpp
  include/token_type.h  
)

target_include_directories(tokenizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(tokenizer
  PUBLIC interpreter
)

add_executable(
  tokenizer_test
  test/test.cpp
)

target_link_libraries(
  tokenizer_test
  tokenizer
  config
  util
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tokenizer_test)
