include(CMakePackageConfigHelpers)

add_library(MyStaticLibInner STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(MyStaticLibInner
  PRIVATE
    MyStaticLib.cpp
)

set_target_properties(MyStaticLibInner PROPERTIES PUBLIC_HEADER MyStaticLib.hpp)

target_include_directories(MyStaticLibInner PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# wrap static library to force clients to link in static lib, despite being unreferenced
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MyStaticLib INTERFACE)
set_target_properties(MyStaticLib PROPERTIES LINKER_LANGUAGE CXX)
add_library(MyStaticLib::MyStaticLib ALIAS MyStaticLib)

#TODO: Switch to target_link_options(MyStaticLib INTERFACE LINKER:<flags>)
if(APPLE)
    target_link_libraries(MyStaticLib INTERFACE
        "-Wl,-u,_mystaticlib_init" # C symbols are prefixed with "_" on macOS
        MyStaticLibInner
    )
elseif(EMSCRIPTEN OR ANDROID)
    # Emscripten and Android's linker doesn´t support --require-defined
    target_link_libraries(MyStaticLib INTERFACE
        "-Wl,--undefined=mystaticlib_init"
        MyStaticLibInner)
else()
    target_link_libraries(MyStaticLib INTERFACE
        "-Wl,--require-defined=mystaticlib_init"
        MyStaticLibInner
    )
endif()

# install rules
install(
    TARGETS MyStaticLib MyStaticLibInner
    EXPORT MyStaticLibTargets
)
install(
    EXPORT MyStaticLibTargets
    DESTINATION cmake
    NAMESPACE MyStaticLib::
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MyStaticLibConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMinorVersion
)
install(
    FILES
        MyStaticLibConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/MyStaticLibConfigVersion.cmake
    DESTINATION cmake
)
