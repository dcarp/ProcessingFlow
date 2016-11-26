enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_processing_node test_processing_node.cpp)
target_link_libraries(test_processing_node ${GTEST_BOTH_LIBRARIES} tbb)

add_test(TestProcessingNode test_processing_node)
