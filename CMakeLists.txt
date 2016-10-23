cmake_minimum_required(VERSION 3.5)

project(ProcessingFlow)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(FlatBuffers REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(include)
add_subdirectory(src)
