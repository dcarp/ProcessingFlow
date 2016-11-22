cmake_minimum_required(VERSION 2.8)

project(ProcessingFlow CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_definitions(-DTBB_USE_GLIBCXX_VERSION=60201)
#     add_definitions(-stdlib=libc++)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(OpenCV REQUIRED)

enable_testing()

include_directories(
    include
    ${CMAKE_BINARY_DIR}/include
    ${FLATBUFFERS_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)
