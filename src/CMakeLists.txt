build_flatbuffers(
    "${CMAKE_CURRENT_SOURCE_DIR}/schema/opencv.fbs"
    ""
    schemas
    ""
    ${CMAKE_BINARY_DIR}/include
    "" "")

include_directories(${CMAKE_BINARY_DIR}/include)

add_executable(processing_flow main.cpp opencv_utils.cpp)
add_dependencies(processing_flow schemas)
target_link_libraries(processing_flow tbb)
