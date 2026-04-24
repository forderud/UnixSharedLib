cmake_minimum_required(VERSION 3.28)

project(TestProject)

# enable optimizations
add_compile_options(-O3)

# eliminate dead code
if (APPLE)
    add_link_options("LINKER:-dead_strip")
    add_link_options("LINKER:-dead_strip_dylibs")
else()
    add_compile_options("-ffunction-sections" "-fdata-sections")
    add_link_options("LINKER:--gc-sections")
endif()

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

add_subdirectory(MySharedLib)

add_subdirectory(MyExecutable)

if (APPLE)
    add_subdirectory(ParseMach)
else()
    add_subdirectory(ParseELF)
endif()
