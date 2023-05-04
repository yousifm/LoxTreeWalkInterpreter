add_library(
  util SHARED
  include/file.h
  src/file.cpp
)

target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
