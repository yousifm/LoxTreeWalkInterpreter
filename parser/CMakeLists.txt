add_library(
  parser
  include/parser.h
  src/parser.cpp
)

target_include_directories(parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(parser
  PUBLIC expression
  PUBLIC token
  PUBLIC statement
)

set_target_properties(parser PROPERTIES LINKER_LANGUAGE CXX)
