add_library(interpreter
  src/interpreter.cpp
  src/lox_function.cpp
)

target_include_directories(interpreter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
  interpreter
  PRIVATE expression
  PRIVATE lox
)
