add_executable(processing_flow main.cpp opencv_utils.cpp)
add_dependencies(processing_flow schemas)
target_link_libraries(processing_flow tbb)

add_subdirectory(schema)
