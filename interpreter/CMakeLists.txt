add_library(interpreter
  src/interpreter.cpp
)

target_include_directories(interpreter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
  interpreter
  PUBLIC type
  PRIVATE expression
  PRIVATE lox
)
