cmake_minimum_required(VERSION 3.22)

project(TestProject)

# eliminate dead code
if (APPLE)
    add_link_options("LINKER:-dead_strip")
    add_link_options("LINKER:-dead_strip_dylibs")
else()
    add_compile_options("-ffunction-sections" "-fdata-sections")
    add_link_options("LINKER:--gc-sections")
endif()

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

if(NOT EMSCRIPTEN)
    add_subdirectory(MySharedLib)
endif()

add_subdirectory(MyStaticLib)

add_subdirectory(MyExecutable)
