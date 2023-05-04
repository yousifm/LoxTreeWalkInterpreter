cmake_minimum_required(VERSION 3.14...3.26)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(Interpreter)

add_subdirectory(interpreter)
add_subdirectory(tokenizer)
add_subdirectory(expression)
add_subdirectory(util)

add_executable(Interpreter src/main.cpp)

target_link_libraries(Interpreter PRIVATE interpreter)
set_target_properties(Interpreter PROPERTIES LINKER_LANGUAGE CXX)
