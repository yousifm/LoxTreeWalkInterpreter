add_library(type
  src/lox_function.cpp
  src/lox_type.cpp
  src/lox_class.cpp
  src/lox_instance.cpp
)

target_include_directories(type PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(type PRIVATE interpreter PRIVATE statement)
