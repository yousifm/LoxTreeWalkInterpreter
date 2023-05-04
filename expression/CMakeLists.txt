add_library(
  expression
  include/expr.h
  include/binary.h
  include/grouping.h
  include/literal.h
  include/unary.h
)

set_target_properties(expression PROPERTIES LINKER_LANGUAGE CXX)
