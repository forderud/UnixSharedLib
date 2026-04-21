
cmake_minimum_required(VERSION 4.3)

project(TestProject)

# Enable optimizations for release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory(MyDynFramework)
add_subdirectory(MyExecutable)
add_subdirectory(ParseMach)
