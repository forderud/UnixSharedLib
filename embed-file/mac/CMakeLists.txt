cmake_minimum_required(VERSION 4.3)

project(TestProject)

# enable optimizations
add_compile_options(-O3)

# eliminate dead code
add_compile_options(-ffunction-sections -fdata-sections)
if (APPLE)
    add_link_options("LINKER:-dead_strip")
else()
    add_link_options("LINKER:--gc-sections")
endif()

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

add_subdirectory(MyDynFramework)
add_subdirectory(MyExecutable)
add_subdirectory(ParseMach)
