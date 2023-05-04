get_filename_component(TEST_RESOURCE_PATH ${CMAKE_SOURCE_DIR}/resource/test/ ABSOLUTE)
configure_file(config.h.in config.h)

add_library(config config.h.in)
target_include_directories(config PUBLIC ${CMAKE_BINARY_DIR}/config)

set_target_properties(config PROPERTIES LINKER_LANGUAGE CXX)
