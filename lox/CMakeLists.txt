add_library(
  lox
  include/lox.h
  src/lox.cpp
)

target_include_directories(lox PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(lox
  PUBLIC util
  PUBLIC tokenizer
  PUBLIC parser
)
