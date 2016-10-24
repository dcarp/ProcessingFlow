enable_testing()
find_package(GTest REQUIRED)

add_executable(test_processing_node test_processing_node.cpp)
target_link_libraries(test_processing_node GTest::GTest GTest::Main tbb)

add_test(TestProcessingNode test_processing_node)
