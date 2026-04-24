cmake_minimum_required(VERSION 4.3)

project(TestProject)

# enable optimizations
add_compile_options(-O3)

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

add_subdirectory(MyDynFramework)
add_subdirectory(MyExecutable)
add_subdirectory(ParseMach)
