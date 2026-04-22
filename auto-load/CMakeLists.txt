cmake_minimum_required(VERSION 3.31)

project(TestProject)

# Enable optimizations for release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
