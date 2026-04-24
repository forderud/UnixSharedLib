cmake_minimum_required(VERSION 3.28)

project(TestProject)

# eliminate dead code
add_compile_options(-ffunction-sections -fdata-sections)
if (APPLE)
    add_link_options("LINKER:-dead_strip")
else()
    add_link_options("LINKER:--gc-sections")
endif()

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
