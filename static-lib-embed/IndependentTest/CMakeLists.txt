cmake_minimum_required(VERSION 3.22)

project(TestProject)

add_executable(IndependentTest
    ../MyExecutable/Main.cpp # reuse sources
)

find_package(StaticLibWrap REQUIRED)
target_link_libraries(IndependentTest PRIVATE StaticLibWrap::StaticLibWrap)

if (IOS)
    set_target_properties(IndependentTest PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER IndependentTest
    )
endif()
