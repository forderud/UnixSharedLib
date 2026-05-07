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

add_library(MyStaticLib::MyStaticLib ALIAS MyStaticLib)

if(APPLE)
    target_link_options(MyStaticLib INTERFACE
        "LINKER:-u,_mystaticlib_init" # C symbols are prefixed with "_" on macOS
    )
elseif(EMSCRIPTEN OR ANDROID)
    # Emscripten and Android's linker doesn´t support --require-defined
    target_link_options(MyStaticLib INTERFACE
        "-Wl,--undefined=mystaticlib_init"
    )
else()
    target_link_options(MyStaticLib INTERFACE
        "-Wl,--require-defined=mystaticlib_init"
    )
endif()

# install rules
install(
    TARGETS MyStaticLib
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
