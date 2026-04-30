include(CMakePackageConfigHelpers)

add_library(MyStaticLib STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(MyStaticLib
  PRIVATE
    MyStaticLib.cpp
)

set_target_properties(MyStaticLib PROPERTIES PUBLIC_HEADER MyStaticLib.hpp)

target_include_directories(MyStaticLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
