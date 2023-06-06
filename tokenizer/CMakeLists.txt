add_library(
  token
  src/token.cpp
)

target_include_directories(token PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
  token
  PUBLIC type
)

add_library(
  tokenizer
  src/tokenizer.cpp
)

target_include_directories(tokenizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(tokenizer
  PUBLIC type
  PUBLIC token
  PUBLIC lox
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
