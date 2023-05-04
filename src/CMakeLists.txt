file(GLOB headers CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/interpreter/*.h ${CMAKE_SOURCE_DIR}/include/interpreter/expression/*.h)

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(includes ${sources} ${headers})

target_include_directories(includes PUBLIC ${CMAKE_SOURCE_DIR}/include/interpreter)
