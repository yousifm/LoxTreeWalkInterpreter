add_library(
  expression
  include/expr.h
  include/binary.h
  include/grouping.h
  include/literal.h
  include/unary.h
)

target_include_directories(expression PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(expression PUBLIC tokenizer)

set_target_properties(expression PROPERTIES LINKER_LANGUAGE CXX)
