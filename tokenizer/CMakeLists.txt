add_library(
  tokenizer
  include/tokenizer.h
  src/tokenizer.cpp
  include/token.h
  src/token.cpp
  include/token_type.h  
)

target_include_directories(tokenizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tokenizer PUBLIC interpreter)
