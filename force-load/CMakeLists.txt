cmake_minimum_required(VERSION 3.28)

project(TestProject)

# strip away unreferenced symbols
add_compile_options(-ffunction-sections -fdata-sections)
if (APPLE)
    add_link_options("-Wl,-dead_strip")
else()
    add_link_options("-Wl,--gc-sections")
endif()

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
