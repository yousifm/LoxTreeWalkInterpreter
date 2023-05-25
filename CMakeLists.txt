cmake_minimum_required(VERSION 3.14...3.26)

project(LoxTreeWalk)

set(CMAKE_CXX_COMPILER /usr/bin/c++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(config)

add_subdirectory(lox)
add_subdirectory(tokenizer)
add_subdirectory(expression)
add_subdirectory(statement)
add_subdirectory(util)
add_subdirectory(parser)
add_subdirectory(interpreter)

add_executable(LoxTreeWalk src/main.cpp)

target_link_libraries(LoxTreeWalk PRIVATE lox PRIVATE expression)

set_target_properties(LoxTreeWalk PROPERTIES LINKER_LANGUAGE CXX)
