enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_processing_node test_processing_node.cpp)
target_link_libraries(test_processing_node ${GTEST_BOTH_LIBRARIES} tbb pthread)

if(${SHOW_EVERY_TEST})
    GTEST_ADD_TESTS(test_processing_node "" test_processing_node.cpp)
else()
    add_test(TestProcessingNode test_processing_node)
endif()
