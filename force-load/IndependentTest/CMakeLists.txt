cmake_minimum_required(VERSION 3.28)

project(TestProject)

add_executable(IndependentTest
    Main.cpp
)

#find_package(MySharedLib REQUIRED)
#target_link_libraries(IndependentTest PRIVATE MySharedLib)

#find_package(MyStaticLib REQUIRED)
#target_link_libraries(IndependentTest PRIVATE MyStaticLib)
