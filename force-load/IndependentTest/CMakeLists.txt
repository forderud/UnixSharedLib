cmake_minimum_required(VERSION 3.28)

project(TestProject)

add_executable(IndependentTest
    ../MyExecutable/Main.cpp # reuse sources
)

if(NOT EMSCRIPTEN)
    find_package(MySharedLib REQUIRED)
    target_link_libraries(IndependentTest PRIVATE MySharedLib::MySharedLib)
endif()

find_package(MyStaticLib REQUIRED)
target_link_libraries(IndependentTest PRIVATE MyStaticLib::MyStaticLib)
