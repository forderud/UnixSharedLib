cmake_minimum_required(VERSION 3.31)

project(TestProject)

# enable optimizations
add_compile_options(-O3)
# enable LTO optimizations
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
