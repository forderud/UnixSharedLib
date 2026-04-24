cmake_minimum_required(VERSION 3.31)

project(TestProject)

# enable optimizations
add_compile_options(-O3)

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
