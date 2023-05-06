add_library(
  expression
  include/expr.h
  src/expr.cpp
  include/expression_visitor.h
  include/printer_visitor.h
  src/printer_visitor.cpp
)

target_include_directories(expression PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(expression PUBLIC tokenizer)

set_target_properties(expression PROPERTIES LINKER_LANGUAGE CXX)
