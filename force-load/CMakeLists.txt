cmake_minimum_required(VERSION 3.28)

project(TestProject)

# strip away unreferenced symbols
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_subdirectory(MySharedLib)
add_subdirectory(MyStaticLib)
add_subdirectory(MyExecutable)
