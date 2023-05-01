cmake_minimum_required(VERSION 3.14...3.26)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(Interpreter)

add_subdirectory(src)

add_executable(Interpreter main.cpp)

target_link_libraries(Interpreter PRIVATE includes)
