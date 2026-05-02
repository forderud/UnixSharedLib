include(CMakePackageConfigHelpers)

add_library(InnerStaticLib STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(InnerStaticLib
  PRIVATE
    InnerStaticLib.cpp
)

set_target_properties(InnerStaticLib PROPERTIES PUBLIC_HEADER InnerStaticLib.hpp)

target_include_directories(InnerStaticLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(FALSE)
    # install rules
    install(
        TARGETS InnerStaticLib
        EXPORT InnerStaticLibTargets
    )
    install(
        EXPORT InnerStaticLibTargets
        DESTINATION cmake
        NAMESPACE InnerStaticLib::
    )
    
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/InnerStaticLibConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMinorVersion
    )
    install(
        FILES
            InnerStaticLibConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/InnerStaticLibConfigVersion.cmake
        DESTINATION cmake
    )
endif()
