cmake_minimum_required(VERSION 3.28)

project(TestProject)

add_executable(IndependentTest
    ../MyExecutable/Main.cpp # reuse sources
)

find_package(MySharedLib REQUIRED)
target_link_libraries(IndependentTest PRIVATE MySharedLib::MySharedLib)

find_package(MyStaticLib REQUIRED)
target_link_libraries(IndependentTest PRIVATE MyStaticLib::MyStaticLib)
