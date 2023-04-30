file(GLOB headers CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/interpreter/*.hpp)
file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(includes ${sources} ${headers})

target_include_directories(includes PUBLIC ${CMAKE_SOURCE_DIR}/include/interpreter)
