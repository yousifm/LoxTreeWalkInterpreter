add_library(
  statement
  include/stmt.h
  src/stmt.cpp
)

target_include_directories(statement PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(statement PUBLIC expression)
