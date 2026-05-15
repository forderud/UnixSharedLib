cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)

project(TestProject)

# enable optimizations
add_compile_options(-O3)

# use dlopen to load shared libs at runtime
#add_compile_definitions(USE_DLOPEN)

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
