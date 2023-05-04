add_library(
  interpreter
  include/interpreter.h
  src/interpreter.cpp
)

target_include_directories(interpreter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(interpreter
  PUBLIC util
  PUBLIC tokenizer)
