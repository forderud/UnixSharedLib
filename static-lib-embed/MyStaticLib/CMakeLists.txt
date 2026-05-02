include(CMakePackageConfigHelpers)

add_library(MyStaticLib STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(MyStaticLib
  PRIVATE
    MyStaticLib.cpp
)

set_target_properties(MyStaticLib PROPERTIES PUBLIC_HEADER MyStaticLib.hpp)

target_link_libraries(MyStaticLib PRIVATE InnerStaticLib)

target_include_directories(MyStaticLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(FALSE)
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
endif()
